import cgi
import http.server # TODO use more secure webserver - https://docs.python.org/3/library/http.server.html#http-server-security
import os
import random
import socketserver
import subprocess
import zipfile

PORT = 8080
DIRECTORY = os.getcwd()
UPLOADS = os.path.join(DIRECTORY, "uploads")
password = os.environ.get("PASSWORD", "password").encode()

def is_encrypted(zip_path):
    try:
        zf = zipfile.ZipFile(zip_path)
        for zinfo in zf.infolist():
            is_encrypted = zinfo.flag_bits & 0x1 
            return is_encrypted
    except:
        return False

def unzipFile(basepath):
    archive = os.path.join(basepath, "files.zip")
    extract_dir = basepath
    if not is_encrypted(archive):
        return (400, "we're not gonna waste any cycles on non-Romesanware(tm)'d files")
    try:
        result = subprocess.run(
            ["unzip", "-P", password.decode(), archive, "-d", extract_dir],
            capture_output=True, text=True
        )
        if result.returncode != 0:
            return (500, f"unzip error:\n{result.stderr}")

        text = ""
        for root, _, files in os.walk(extract_dir):
            for fname in files:
                full = os.path.join(root, fname)
                if full == archive:
                    continue
                try:
                    with open(full, 'r', encoding='utf-8', errors='replace') as f:
                        text += "here are your files!"
                        text += f"\n--- {full} ---\n"
                        text += f.read()
                except Exception as e:
                    text += f"\n[!] error reading {full}: {e}\n"

        return (200, text)
    except Exception as e:
        return (500, f"system unzip failed: {e}")

class UploadHandler(http.server.SimpleHTTPRequestHandler):
    def do_POST(self):
        if self.path != '/upload':
            self.send_error(404, "not found")
            return

        ctype, pdict = cgi.parse_header(self.headers.get('Content-Type', ''))
        if ctype != 'multipart/form-data':
            self.send_error(400, "not multipart")
            return

        pdict['boundary'] = bytes(pdict['boundary'], "utf-8")
        pdict['CONTENT-LENGTH'] = int(self.headers['Content-Length'])

        form = cgi.FieldStorage(
            fp=self.rfile,
            headers=self.headers,
            environ={'REQUEST_METHOD': 'POST'},
            keep_blank_values=True
        )
        if 'file' not in form:
            self.send_error(400, "no file field")
            return

        data = form['file'].file.read()
        rand = random.randbytes(8).hex()
        basepath = os.path.join(UPLOADS, rand)
        os.makedirs(basepath, exist_ok=True)
        archive_path = os.path.join(basepath, "files.zip")
        open(archive_path, 'wb').write(data)

        code, content = unzipFile(basepath)
        self.send_response(code)
        self.end_headers()
        self.wfile.write(content.encode('utf-8'))

    def translate_path(self, path):
        p = super().translate_path(path)
        rel = os.path.relpath(p, os.getcwd())
        return os.path.join(DIRECTORY, rel)

    def list_directory(self, path):
        self.send_error(403, "no peeking")
        return None

with socketserver.TCPServer(("", PORT), UploadHandler) as httpd:
    print(f"serving {DIRECTORY} at http://localhost:{PORT}")
    httpd.serve_forever()
