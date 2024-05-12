import unittest
import json
import requests
import sys
HOST="127.0.0.1"
URL = "http://" + HOST + ":8000/api/v1/persons"

p1 = {"name" : "katya",
      "mobilePhone" : "22",
      "login" : "moscow",
      "email" : "huawei"}
p2 = {"name" : "katya_2",
      "mobilePhone" : "23",
      "login" : "moscow_2",
      "email" : "huawei_2"}
p3 = {"name" : "katya_3",
      "mobilePhone" : "24",
      "login" : "moscow_3",
      "email" : "huawei_3"
      }
p_upd = {"name" : "katya_u",
         "mobilePhone" : "25",
         "login" : "moscow_u",
         "email" : "huawei_u"
                   }
p_inv = {"nane" : "katya",
         "email" : "huawei"}
p1j = json.dumps(p1)
p2j = json.dumps(p2)
p3j = json.dumps(p3)
p_updj = json.dumps(p_upd)
p_invj = json.dumps(p_inv)
headers = {'Content-type': 'application/json', 'Accept': 'text/'}
class TestMain(unittest.TestCase):
  # positives
  def test_crd(self):
    pid = requests.post(URL, headers=headers, data = p1j).headers["Location"].split("/")[4]
    data = requests.get(URL + "/" + str(pid), headers=headers).json()

    status = requests.delete(URL + "/" + str(pid), headers=headers).status_code
    exp = status == 204 and data["name"] == p1["name"] and data["mobilePhone"] == p1["mobilePhone"] and data["login"] == p1["login"] and data["email"] == p1["email"]
    self.assertEqual(exp, True)
  def test_get_all(self):
    id1 = requests.post(URL, headers=headers, data = p1j).headers["Location"].split("/")[4]
    id2 = requests.post(URL, headers=headers, data = p2j).headers["Location"].split("/")[4]
    id3 = requests.post(URL, headers=headers, data = p3j).headers["Location"].split("/")[4]
    
    data = requests.get(URL, headers=headers).json()
    exp = len(data) == 3

    requests.delete(URL + "/" + str(id1), headers=headers)
    requests.delete(URL + "/" + str(id2), headers=headers)
    requests.delete(URL + "/" + str(id3), headers=headers)
    self.assertEqual(exp, True)
  def test_upd(self):
    id1 = requests.post(URL, headers=headers, data = p1j).headers["Location"].split("/")[4]
    
    r = requests.patch(URL + "/" + str(id1), headers=headers, data = p_updj)
    data = r.json()
    exp = data["name"] == p_upd["name"] and data["mobilePhone"] == p_upd["mobilePhone"] and data["login"] == p_upd["login"] and data["email"] == p_upd["email"]

    requests.delete(URL + "/" + str(id1), headers=headers)
    self.assertEqual(exp, True)
# negative
  def test_get_neg(self):
    r = requests.get(URL + "/" + str(503737), headers=headers)
    exp = r.status_code == 404
    self.assertEqual(exp, True)
  def test_upd_inv_data(self):
    id1 = requests.post(URL, headers=headers, data = p1j).headers["Location"].split("/")[4]
    r = requests.patch(URL + "/" + str(id1), headers=headers, data = p_invj)
    exp = r.status_code == 400
    requests.delete(URL + "/" + str(id1), headers=headers)
    self.assertEqual(exp, True)
  def test_upd_inv_type_id(self):
    id1 = requests.post(URL, headers=headers, data = p1j).headers["Location"].split("/")[4]
    r = requests.patch(URL + "/" + "smth", headers=headers, data = p_updj)
    exp = r.status_code == 400
    requests.delete(URL + "/" + str(id1), headers=headers)
    self.assertEqual(exp, True)
  def test_upd_inv_id(self):
    r = requests.patch(URL + "/" + "4579827", headers=headers, data = p_updj)
    exp = r.status_code == 404
    self.assertEqual(exp, True)
  def test_del_inv_type_id(self):
    r = requests.delete(URL + "/" + "smth", headers=headers)
    exp = r.status_code == 400
    self.assertEqual(exp, True)
  def test_del_inv_id(self):
    r = requests.delete(URL + "/" + "4579827", headers=headers)
    exp = r.status_code == 404
    self.assertEqual(exp, True)
  def test_create_inv_data(self):
    r = requests.post(URL, headers=headers, data = p_invj)
    exp = r.status_code == 400
    self.assertEqual(exp, True)
  def test_get_inv_type_id(self):
    r = requests.get(URL + "/" + "smth", headers=headers)
    exp = r.status_code == 400
    self.assertEqual(exp, True)
  def test_get_inv_id(self):
    r = requests.get(URL + "/" + "4579827", headers=headers)
    exp = r.status_code == 404

if len(sys.argv) > 1:
  HOST = str(sys.argv[1])
  URL = "http://" + HOST + ":8000/api/v1/persons"
  del sys.argv[1:]
unittest.main()
    
