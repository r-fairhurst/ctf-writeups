from flask import Flask, request, render_template_string, send_from_directory
from PIL import Image, ImageDraw, ImageFont
from datetime import datetime
import os

app = Flask(__name__)

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        caption = request.form["text"]

        date = request.form["date"]
        
        img = Image.open("./shrek.jpg").convert("RGBA")
        fnt = ImageFont.load_default(48)
        cap = Image.new("RGBA", img.size, (255, 255, 255, 0))
        d = ImageDraw.Draw(cap)
        d.multiline_text((10, 10), caption, font=fnt, fill=(0, 0, 0), font_size=64.0)

        # add caption
        combined = Image.alpha_composite(img, cap)

        filename = os.urandom(8).hex()
        combined.convert("RGB").save("img/" + filename + ".jpg")

        with open(f"img/{filename}.txt", 'w') as f:
            f.write(date)
        return f"image available at <a href='img/{filename}'>img/{filename}</a>"

    return f'''
    <form method=post enctype=multipart/form-data>
      meme text: <input type=text name=text><br>
      <input type=hidden name=date value="{datetime.utcnow().isoformat()}"><br>
      <input type=submit value=make>
    </form>
    '''

@app.route("/img/<path:filename>")
def serve_img(filename):
    img_path = os.path.join("img", filename) + ".jpg"
    txt_path = os.path.join("img", filename) + ".txt"

    if not os.path.isfile(img_path):
        return "Image not found", 404

    alt_text = "Alt text not found"
    if os.path.isfile(txt_path):
        with open(txt_path, "r") as f:
            alt_text = f.read().strip()

    image_url = f"/serve_img/{filename}.jpg"
    
    print(alt_text)

    return render_template_string(f"""
        <img src="{image_url}" alt="{alt_text}">
    """)


@app.route("/serve_img/<path:filename>")
def send_image(filename):
    return send_from_directory("img", filename)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=1337)
