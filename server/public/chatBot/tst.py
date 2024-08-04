
from browser import bind, document, ajax
import json

@bind("#open-chat", "click")
def openChatBot(event):
    display = document["chat"].style.display
    document["chat"].style.display = "block" if display == "none" else "none"

@bind("#send", "click")
def sendQuestion(event):
    question = document["chat-box"].value
    askAi(question)
    
GOOGLE_API_KEY = "AIzaSyAiz3ep3Hae-zzryQoUN5iI3CuYnNB-2M8"
url = f"https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key={GOOGLE_API_KEY}"
headers = {'Content-Type': 'application/json'}

def handle_response(req):
    if req.status == 200:
        data = req.json
        # Process the data received from the API
        print(data)
    else:
        print('Error:', req.text)
def askAi(question):        
    data = {
        "contents": [{
            "role": "user",
            "parts": [{"text": f"{question}"}]
        }],
        "system_instruction": {
            "parts": [{"text": "You are a chat bot on an e-commerce site, give brief answer."}]
        },
    }
    ajax.post(url, data=json.dumps(data), headers=headers, oncomplete=handle_response)