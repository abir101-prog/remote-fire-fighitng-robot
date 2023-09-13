import requests
import json
url = 'http://192.168.0.102:80/data'
data = {
    "key1": "value1",
    "key2": 42
}

headers = {
    "Content-Type": "application/json"
}
x = requests.post(url, data=json.dumps(data), headers=headers) #json = myobj)

print(x.text)