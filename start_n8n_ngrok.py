import subprocess
import time
import requests
import pyperclip
import sys

NGROK_PORT = "5678"
NGROK_API = "http://127.0.0.1:4040/api/tunnels"

def start_ngrok():
    print("🚀 Starting ngrok...")
    return subprocess.Popen(
        ["ngrok", "http", NGROK_PORT],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )

def wait_for_url():
    print("⏳ Waiting for public URL...")
    for _ in range(20):
        try:
            res = requests.get(NGROK_API).json()
            tunnels = res.get("tunnels", [])
            for t in tunnels:
                if t.get("proto") == "https":
                    return t["public_url"]
        except:
            pass
        time.sleep(1)
    return None

def main():
    ngrok_process = start_ngrok()

    try:
        public_url = wait_for_url()
        if not public_url:
            print("❌ Failed to get ngrok URL")
            sys.exit(1)

        pyperclip.copy(public_url)

        print("\n✅ n8n is now public!")
        print(f"🌍 Public URL: {public_url}")
        print("📋 URL copied to clipboard")
        print("\nPress CTRL+C to stop ngrok\n")

        ngrok_process.wait()

    except KeyboardInterrupt:
        print("\n🛑 Stopping ngrok...")
        ngrok_process.terminate()
        sys.exit(0)

if __name__ == "__main__":
    main()
