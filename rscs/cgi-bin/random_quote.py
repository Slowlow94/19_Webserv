#!/usr/bin/env python3
import os
import random
import cgi
import cgitb

cgitb.enable()

sentences = {
    1: "Le courage est la première des qualités humaines.",
    2: "La persévérance est un raccourci vers le succès.",
    3: "Rien ne se perd, rien ne se cree, tout se transforme.",
    4: "Chaque jour est une nouvelle opportunité.",
    5: "Le succès est la somme de petits efforts répétés jour après jour.",
    6: "L'amour a ses raisons que la raison ignore",
    7: "Ve rossi vie macral",
    8: "Ik weet het niet",
    9: "Le savoir, c'est le pouvoir",
    10: "Un peu d'eau fraiche et de verdure, que nous prodigue la nature et vous serez un ours bien leché.",
    11: "Savoir donner, c'est savoir recevoir",
    12: "La sagesse n'a pas d'âge",
    13: "Garde la peche",
    14: "Ce n'est que dos au mur qu'on se rend compte qu'il faut avancer",
    15: "J'ai plus de reseau, j'passe sous un train",
    16: "Plus le pas est lent, moins tu avances"
}

def get_random_sentence(sentences_dict): 
    try: 
        if not isinstance(sentences_dict, dict): 
            raise ValueError("Input must be a dictionary.") 
        if not sentences_dict: 
            raise ValueError("Dictionary is empty.") 
        key = random.choice(list(sentences_dict.keys())) 
        return sentences_dict[key] 
    except ValueError as ve: 
        return f"Error: {ve}"

def get_available_images(directory):
    try:
        if not os.path.exists(directory):
            return []
        return [
            f for f in os.listdir(directory) 
            if os.path.isfile(os.path.join(directory, f)) 
            and os.access(os.path.join(directory, f), os.R_OK)
            and f.lower().endswith(('.png', '.jpg', '.jpeg', '.gif', '.bmp', '.webp'))
        ]
    except Exception as e:
        return []

def generate_image_html(images, IMAGES_DIR):
    return "\n".join(
        f'<img src="{IMAGES_DIR}/{image}" alt="{image}" onclick="selectImage(\'{image}\')" title="{image}">'
        for image in images
    )

def main():
    form = cgi.FieldStorage()
    method = os.environ.get("REQUEST_METHOD", "GET").upper()
    query = os.environ.get("QUERY_STRING", "")
    path = os.environ.get("PATH", "")
    IMAGES_DIR = os.environ.get("IMAGESPATH", "")
    clean_image = IMAGES_DIR.replace(path, "")
    SCRIPTPATH = os.environ.get("SCRIPT_NAME", "")
    clean_path = SCRIPTPATH.replace(path, "")

    if method == "POST":
        file_item = form["image"]
        if file_item.filename:
            filename = os.path.basename(file_item.filename)
            upload_path = os.path.join(IMAGES_DIR, filename)
        else:
            message = "No selected file to upload."
        
        html_content = f"""
        <!DOCTYPE html>
        <html lang="en">
        <body>
            <h1>{message}</h1>
            <a href="/">Return Home</a>
        </body>
        </html>
        """

    else:
        if query:
            selected_image = form.getvalue("image", "No selected image")
            random_phrase = get_random_sentence(sentences)

            image_path = os.path.join(IMAGES_DIR, selected_image)
            if not os.path.isfile(image_path):
                selected_image = None

            html_content = f"""
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <style>
                    body {{
                        font-family: Arial, sans-serif;
                        text-align: center;
                        margin: 20px;
                    }}
                    h1 {{
                        color: #333;
                    }}
                    .image-container {{
                        position: relative;
                        display: inline-block;
                        margin: 20px;
                    }}
                    .image-container img {{
                        max-width: 100%;
                        height: auto;
                        border: 2px solid #ddd;
                        border-radius: 8px;
                        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
                    }}
                    .image-container .caption {{
                        position: absolute;
                        top: 50%;
                        left: 50%;
                        transform: translate(-50%, -50%);
                        background-color: rgba(0, 0, 0, 0.6);
                        color: white;
                        padding: 5px 10px;
                        border-radius: 5px;
                        font-size: 18px;
                        text-align: center;
                        white-space: nowrap;
                    }}
                    .home-link {{
                        display: inline-block;
                        margin-top: 20px;
                        padding: 10px 20px;
                        font-size: 16px;
                        color: white;
                        background-color: #007bff;
                        text-decoration: none;
                        border-radius: 5px;
                    }}
                    .home-link:hover {{
                        background-color: #0056b3;
                    }}
                </style>
            </head>
            <body>

                <!-- Conteneur de l'image et du texte -->
                <div class="image-container">
                    {"<img src='" + clean_image + "/" + selected_image + "' alt='Selected Image'>" if selected_image else "<p>No selected image</p>"}
                    <div class="caption">{random_phrase}</div>
                </div>

                <a href="/" class="home-link">Return Home</a>
            </body>
            </html>
            """
        else:
            available_images = get_available_images(IMAGES_DIR)
            image_html = generate_image_html(available_images, clean_image)
            image_section_style = "" if image_html else 'style="display:none;"'

            html_content = f"""
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Choose an Image</title>
                <style>
                    body {{
                        font-family: Arial, sans-serif;
                        margin: 20px;
                    }}
                    h1 {{
                        text-align: center;
                        color: #333;
                    }}
                    .image-preview {{
                        display: flex;
                        justify-content: center;
                        flex-wrap: wrap;
                        gap: 20px;
                        margin: 20px 0;
                    }}
                    .image-preview img {{
                        width: 150px;
                        height: 150px;
                        object-fit: cover;
                        cursor: pointer;
                        border: 2px solid transparent;
                        transition: transform 0.3s, border 0.3s;
                    }}
                    .image-preview img:hover {{
                        transform: scale(1);
                        border: 2px solid #007bff;
                    }}
                    #submit-button {{
                        margin: 20px auto;
                        display: block;
                        padding: 10px 20px;
                        font-size: 16px;
                        background-color: #007bff;
                        color: white;
                        border: none;
                        cursor: pointer;
                    }}
                    #submit-button:hover {{
                        background-color: #0056b3;
                    }}
                    input[type="file"] {{
                        margin: 10px 0;
                    }}
                </style>
            </head>
            <body>
                <div><a href='/'>Home</a></div>

                <h1>KitshGI</h1>

                <div class="section" {image_section_style}>
                    <h2> Choose your favorite one </h2>
                    <form id="image-form" action="{clean_path}" method="GET">
                        <input type="hidden" id="selected-image" name="image" value="">

                        <!-- Aperçu des images disponibles -->
                        <div class="image-preview">
                            {image_html}
                        </div>

                        <button type="submit" id="submit-button" disabled>Submit</button>
                    </form>
                </div>

                <div class="section">
                    <h2>Upload a New Image</h2>
                    <form action="{clean_path}" method="POST" enctype="multipart/form-data">
                        <label for="image">Choose an image to upload:</label><br>
                        <input type="file" id="image" name="image" accept="image/*"><br>
                        <button type="submit" id="upload-button" disabled>Upload</button>
                    </form>
                </div>
                <script>
                    function selectImage(imageName) {{
                        document.getElementById('selected-image').value = imageName;
                        const images = document.querySelectorAll('.image-preview img');
                        images.forEach(img => img.style.border = "2px solid transparent");
                        const selectedImage = document.querySelector(`img[alt="${{imageName}}"]`);
                        if (selectedImage) {{
                            selectedImage.style.border = "2px solid green";
                            const submitButton = document.getElementById('submit-button');
                            submitButton.disabled = false;
                        }} else {{
                            console.error("Image not found:", imageName);
                        }}
                    }}
                </script>
                <script>
                    const fileInput = document.getElementById('image');
                    const Upload = document.getElementById('upload-button');
                    fileInput.addEventListener('change', () => {{
                        if (fileInput.files.length > 0) {{
                            Upload.disabled = false;
                        }} else {{
                            Upload.disabled = true;
                        }}
                    }});
                </script>
            </body>
            </html>
            """

    print("Content-Type: text/html\r\n\r\n")
    print(html_content)

if __name__ == "__main__":
    main()
