import subprocess
import time
import json
import urllib.request
import pyperclip
import os

def launch():
    print("--- Starting ngrok ---")
    # Start ngrok in the background
    ngrok_proc = subprocess.Popen(["ngrok", "http", "5678"], stdout=subprocess.PIPE)
    
    # Give ngrok a moment to initialize and fetch a URL
    time.sleep(3)
    
    try:
        # ngrok has a local API we can query to find the public URL
        with urllib.request.urlopen("http://127.0.0.1:4040/api/tunnels") as response:
            data = json.loads(response.read().decode())
            public_url = data['tunnels'][0]['public_url']
            
            print(f"✅ Public URL: {public_url}")
            pyperclip.copy(public_url)
            print("📋 URL copied to clipboard!")
            
            # Set the environment variable for the current process
            os.environ["WEBHOOK_URL"] = public_url + "/"
            
            print("--- Starting n8n ---")
            # This will keep the script running and show n8n logs
            subprocess.run(["n8n", "start"], shell=True)

    except Exception as e:
        print(f"❌ Error: {e}")
        ngrok_proc.terminate()

if __name__ == "__main__":
    launch()
