#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t session_lock = PTHREAD_MUTEX_INITIALIZER;

#define PORT            8888
#define POSTBUFFERSIZE  512
#define MAXNAMESIZE     20
#define MAXANSWERSIZE   512

#define GET             0
#define POST            1

#define THREETLEN 140

#define FLAG_2 "REDACTED"
#define FLAG_3 "REDACTED"
#define FLAG_4 "REDACTED"

const char *errorstr = "<html><body>An internal server error has occurred! </body></html>";

int session_count = 0;
static struct Session *sessions;

// Session information
// Associated with a cookie called "session" with value <sid>
struct Session {
    // Session id
    char sessionid[33];
    // Used bc we use a linked list to store sessions
    struct Session *next;
    // Reference counter for # of sessions using this cookie
    unsigned int rc;
    // Time when this session was last active.
    time_t start;
    // Count threets threeted
    int num_threets;
    // Threet contents
    char threets[3][THREETLEN+1];
    int likes[3];
    char username[THREETLEN+1];
};

struct connection_info_struct
{
  int connectiontype;
  char *answerstring;
  struct Session *session;
  struct MHD_PostProcessor *postprocessor; 
};

// Generate HTML for all threets
char* render_all_threets(struct Session *session) {
    char *html = malloc(session_count*THREETLEN*5+40);
    if (!html) return NULL;
    int len = 0;
    
    len += snprintf(html + len, 1024 - len, "<html><body><h1>All Threets</h1><ul>");
    
    // End before we get to the admin threet
    for (; session->next != NULL; session = session->next) {
        for (int i = 0; i < 3; i++) {
            if (*session->threets[i]) len += snprintf(html + len, 1024 - len, "<li>%s</li>  -<em>%s</em> (%d likes)", session->threets[i], session->username, session->likes[i]);
        }
    }
    
    len += snprintf(html + len, 1024 - len, "</ul></body></html>");
    return html;
}

// Generate HTML for a user's threets
char* render_threets(struct Session *session) {
    char *html = malloc(1024);
    if (!html) return NULL;
    int len = 0;
    
    len += snprintf(html + len, 1024 - len, "<html><body><h1>Your Threets</h1><ul>");
    
    for (int i = 0; i < 3; i++) {
        if (*session->threets[i]) len += snprintf(html + len, 1024 - len, "<li>%s</li>  -<em>%s</em> (%d likes)", session->threets[i], session->username, session->likes[i]);
    }
    
    len += snprintf(html + len, 1024 - len, "</ul>\
                       <form action=\"/threet\" method=\"post\">\
                           <input name=\"threet\" type=\"text\">\
                           <input type=\"submit\" value=\" Send \"></form>\
                    </body></html>");
    return html;
}

// handler to save a threet to a user's session
static int
iterate_post (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
              const char *filename, const char *content_type,
              const char *transfer_encoding, const char *data, 
	      uint64_t off, size_t size)
{
    struct connection_info_struct *con_info = coninfo_cls;
    struct Session *session = con_info->session;

    // Check form data
    if (0 == strcmp (key, "threet")) {
        if ((size > 0) && (size <= THREETLEN)) {
            // Don't allow posting more than 3 threets, any more could overflow!
            if (session->num_threets <= 3) {
                strncpy(session->threets[session->num_threets++], data, size);
            }
            con_info->answerstring = render_threets(session);
        } 
        else con_info->answerstring = NULL;

        return MHD_NO;
    }

    return MHD_YES;
}

// Clean up any allocations
void request_completed (void *cls, struct MHD_Connection *connection, 
        void **con_cls,
        enum MHD_RequestTerminationCode toe)
{
    struct connection_info_struct *con_info = *con_cls;

    if (NULL == con_info) return;
    if (con_info->connectiontype == POST) {
        MHD_destroy_post_processor (con_info->postprocessor);        
        if (con_info->answerstring) free (con_info->answerstring);
    }

    free (con_info);
    *con_cls = NULL;   
}


// Add header to response to set a session cookie.
static void add_session_cookie (struct Session *session,
        struct MHD_Response *response, const char *cookie_name)
{
    char cstr[256];
    snprintf (cstr,
            sizeof (cstr),
            "%s=%s",
            cookie_name,
            session->sessionid);
    if (MHD_NO ==
            MHD_add_response_header (response,
                MHD_HTTP_HEADER_SET_COOKIE,
                cstr))
    {
        fprintf (stderr,
                "Failed to set session cookie header!\n");
    }
}

// Return the session info for a connection or create a new struct if this is a
// new user.
// This struct's sessionid field will be associated with a "session" cookie
static struct Session * get_session (struct MHD_Connection *connection) {
    struct Session *ret;
    const char *cookie;

    // ask MHD to get the cookie for a connection
    cookie = MHD_lookup_connection_value (connection, MHD_COOKIE_KIND, "session");
    if (cookie != NULL) {
        // find matching session
        ret = sessions;
        while (NULL != ret) {
            if (0 == strcmp (cookie, ret->sessionid))
                break;
            ret = ret->next;
        }
        if (NULL != ret) {
            ret->rc++;
            return ret;
        }
    }
    // create fresh session
    ret = calloc (1, sizeof (struct Session));
    if (NULL == ret) {
        fprintf (stderr, "calloc error: %s\n", strerror (errno));
        return NULL;
    }
    // generate sessionid
    snprintf (ret->sessionid,
            sizeof (ret->sessionid),
            "%X%X%X%X",
            (unsigned int) rand (),
            (unsigned int) rand (),
            (unsigned int) rand (),
            (unsigned int) rand ());
    strcpy(ret->username, "anonymous_tree#");
    strncat(ret->username, ret->sessionid, 4);
    ret->rc++;
    ret->start = time (NULL);

    pthread_mutex_lock(&session_lock);
        ret->next = sessions;
        sessions = ret;
    pthread_mutex_unlock(&session_lock);

    session_count ++;
    return ret;
}

// return file descriptor for requested file.
// On not found, return fd 404.
// If that fails or otherwise, consider it a server error
int try_opening_file(const char *url, struct stat *sbuf) {
    int fd;

    // +1 to strip slash
    if ( (-1 == (fd = open(url+1, O_RDONLY))) || (0 != fstat (fd, sbuf)) )
    {
        if (fd != -1) close (fd);
        if (errno == ENOENT && strcmp(url, "/404.html") != 0) {
            fd = try_opening_file("/404.html", sbuf);
        }
    }
    return fd;
}

// Called every time we get a new connection
static int handle_connection (
        void *cls, struct MHD_Connection *connection,
        const char *url, const char *method,
        const char *version, const char *upload_data,
        size_t *upload_data_size, void **con_cls)
{
    // read cookies
    const char *sessionid = MHD_lookup_connection_value (connection, MHD_COOKIE_KIND, "session");
    const char *user = MHD_lookup_connection_value (connection, MHD_COOKIE_KIND, "user");

    // set up POST data processing
    struct Session *session = get_session(connection);
    if (NULL == *con_cls) {
        struct connection_info_struct *con_info;

        con_info = malloc (sizeof (struct connection_info_struct));
        if (NULL == con_info) return MHD_NO;
        con_info->answerstring = NULL;
        con_info->session = session;

        if (0 == strcmp (method, "POST")) {
            // use iterate_post() to process threets
            con_info->postprocessor = MHD_create_post_processor (connection, POSTBUFFERSIZE,
                                        (void *)iterate_post, (void *) con_info);

            if (NULL == con_info->postprocessor) {
                free (con_info);
                return MHD_NO;
            }

            con_info->connectiontype = POST;
        }
        else con_info->connectiontype = GET;
        *con_cls = (void *) con_info;
        return MHD_YES;
    }

    enum MHD_Result ret;
    struct MHD_Response *response;

    // handle GET requests
    if (strcmp (method, "GET") == 0) {
        if (strcmp(url, "/") == 0) url = "/public/index.html";
        int public = 0;
        int fd;
        struct stat sbuf;
        // check if request is for file in /public/ directory
        // we don't want users accessing /secrets.txt!
        if (strncmp("/public/", url, strlen("/public/")) == 0) {
            public = 1;
        }
        // Serve a user's threets
        if (strcmp(url, "/mythreets") == 0) {
            if (session) {
                char *html = render_threets(session);
                if (html) {
                    response = MHD_create_response_from_buffer(strlen(html), html, MHD_RESPMEM_PERSISTENT);
                } else {
                    response = MHD_create_response_from_buffer(strlen(errorstr), (void*) errorstr, MHD_RESPMEM_PERSISTENT);
                    free(html);
                }
            } else {
                response = MHD_create_response_from_buffer(strlen(errorstr), (void*) errorstr, MHD_RESPMEM_PERSISTENT);
            }
        }
        // Verify our VIPs
        else if (strcmp(url, "/verified") == 0) {
            if (user != 0 && strcmp(user, "Thr33lonMusk") == 0) {
                response = MHD_create_response_from_buffer (strlen(FLAG_2), FLAG_2, MHD_RESPMEM_PERSISTENT);
            }
            else if (session->likes[0] == 1633840489 && session->likes[1] == 1633969523 && session->likes[2] == 1919299174) {
                response = MHD_create_response_from_buffer (strlen(FLAG_4), FLAG_4, MHD_RESPMEM_PERSISTENT);
            }
        }
        // Serve the firehose
        else if (strcmp(url, "/threets") == 0) {
            char *html = render_all_threets(sessions);
            if (html) {
                response = MHD_create_response_from_buffer(strlen(html), html, MHD_RESPMEM_PERSISTENT);
            } else {
                response = MHD_create_response_from_buffer(strlen(errorstr), (void*) errorstr, MHD_RESPMEM_PERSISTENT);
                free(html);
            }
        }
        // serve static file
        else {
            fd = try_opening_file(url, &sbuf);
            // Not found
            if (fd < 0 && errno == ENOENT ) fd = try_opening_file("/public/404.html", &sbuf);
            // Found but not public
            else if (public == 0) {
                close(fd);
                fd = try_opening_file("/public/denied.html", &sbuf);
            }
            // internal error
            if (fd < 0) {
                response = MHD_create_response_from_buffer (strlen(errorstr), (void*) errorstr, MHD_RESPMEM_PERSISTENT);
            }
            // send file
            else {
                response = MHD_create_response_from_fd(sbuf.st_size, fd);
            }
        }
    }
    // handle POSTs
    else if (strcmp (method, "POST") == 0 && strcmp(url, "/threet") == 0) {
        struct connection_info_struct *con_info = *con_cls;
        if (*upload_data_size != 0) {
            MHD_post_process (con_info->postprocessor, upload_data,	*upload_data_size);
            *upload_data_size = 0;

            return MHD_YES;
        } 
        else if (NULL != con_info->answerstring) {
            response = MHD_create_response_from_buffer (strlen (con_info->answerstring), con_info->answerstring, MHD_RESPMEM_PERSISTENT);
        }
    }
    if (!sessionid || strcmp(session->sessionid, sessionid) != 0) {
        add_session_cookie(session, response, "session");
    }
    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);

    return ret;
}

// Expire sessions > 5 minutes old
static void expire_sessions() {
    struct Session *pos;
    struct Session *prev;
    struct Session *next;
    time_t now;

    now = time(NULL);
    prev = NULL;
    pos = sessions;
    while (NULL != pos)
    {
        next = pos->next;
        // Skip the tail of the linked list, since that's our admin cookie
        if (next && now - pos->start > 60 * 5) {
            pthread_mutex_lock(&session_lock);
                if (NULL == prev) sessions = pos->next;
                else prev->next = next;
            pthread_mutex_unlock(&session_lock);
            session_count --;
            free (pos);
        }
        else prev = pos;
        pos = next;
    }
}

int main () {
    srand(0);

    struct Session adminSession;
    snprintf (adminSession.sessionid,
            sizeof (adminSession.sessionid),
            "%X%X%X%X",
            (unsigned int) rand (),
            (unsigned int) rand (),
            (unsigned int) rand (),
            (unsigned int) rand ());
    strcpy(adminSession.threets[0], "Just setting up my thrtr.");
    strcpy(adminSession.threets[1], FLAG_3);
    strcpy(adminSession.threets[2], "pretty sure this is the last social media anyone will ever need");
    adminSession.num_threets = 4;
    strcpy(adminSession.username, "thritter_admin");
    adminSession.likes[0] = rand();
    adminSession.likes[1] = rand();
    adminSession.likes[2] = rand();
    adminSession.next = NULL;
    sessions = &adminSession;
    session_count = 1;

    struct MHD_Daemon *daemon;

    // start listening. new thread for each connection
    daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, PORT, NULL, NULL,
            (void *)&handle_connection, NULL, MHD_OPTION_NOTIFY_COMPLETED,
            request_completed, NULL, MHD_OPTION_END);
    if (NULL == daemon)
        return 1;

    // Block until stopped
    // TODO cool this loop down
    while (1) {
        expire_sessions();
        MHD_run (daemon);
        // TODO this is a dumb way to cool the loop down
        sleep(5);
    }
    MHD_stop_daemon (daemon);
    pthread_mutex_destroy(&session_lock);
    return 0;
}

// If prod goes down this is why:
// TODO threet renders could break if the linked list is getting rearranged at that moment
// but whatever
// TODO fix memory leaks
