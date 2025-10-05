import requests
import json

ip = "192.168.0.105"

def test_get_health():
    r = requests.get(f'http://{ip}/v1/device/status')
    assert r.json()[0]["name"] == "Pathfinder"


def test_move_forward():
    r = requests.post(f'http://{ip}/v1/movement-plan', data=json.dumps({
  "name": "Movement Plan",
  "persist": False,
  "steps": [
    {
      "angle": 0,
      "direction": "backward",
      "distance": 1
    }
  ]
}))
    print(r.text)
    move = input("Did it more backwards?")
    assert move =="y"