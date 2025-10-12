import requests
import json
import common


def test_get_health():
    r = requests.get(f'http://{common.ip}/v1/device/status')
    assert r.json()[0]["name"] == "Pathfinder"
