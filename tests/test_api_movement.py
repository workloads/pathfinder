import requests
import json
import common


def test_move_forward():
    r = requests.post(f'http://{common.ip}/v2/movement', data=json.dumps({
  "name": "Movement Plan",
  "persist": False,
  "steps": [
    {
      "angle": 0,
      "direction": "forward",
      "distance": 1
    }
  ]
}))
    print(r.text)
    move = input("Did it move forwards?")
    assert move =="y"


def test_move_backwards():
    r = requests.post(f'http://{common.ip}/v2/movement', data=json.dumps({
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
    move = input("Did it move backwards?")
    assert move =="y"


def test_move_multi():
    r = requests.post(f'http://{common.ip}/v2/movement', data=json.dumps({
  "name": "Movement Plan",
  "persist": False,
  "steps": [
    {
      "angle": 0,
      "direction": "forward",
      "distance": 1
    },
    {
      "angle": 0,
      "direction": "backward",
      "distance": 1
    }
  ]
}))
    print(r.text)
    move = input("Did it move forwards then backwards?")
    assert move =="y"

def test_move_multi_angle():
    r = requests.post(f'http://{common.ip}/v2/movement', data=json.dumps({
  "name": "Movement Plan",
  "persist": False,
  "steps": [
    {
      "angle": 0,
      "direction": "forward",
      "distance": 2
    },
    {
      "angle": 90,
      "direction": "forward",
      "distance": 1
    },
    {
      "angle": 360-90,
      "direction": "backward",
      "distance": 1
    },
    {
      "angle": 0,
      "direction": "backward",
      "distance": 2
    }
  ]
}))
    print(r.text)
    r = requests.post(f'http://{common.ip}/v2/movement/run')
    print(r.text)
    move = input("Did it move forwards then backwards?")
    assert move =="y"