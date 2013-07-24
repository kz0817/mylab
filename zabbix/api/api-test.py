#!/usr/bin/env python

import requests
import json
import sys
import time

test_params = {
  "host":"MylabTestHost",
  "proxy-host":"MylabTestProxy",
  "action-name":"Test Trigger action",
  "maintenance-name":"Test maintenance",
}

group_linux_servers = 2
templateid_os_linux = 10001
OPERATION_TYPE_SEND_MESSAGE = 0
EVENT_SOURCE_TRIGGER = 0
EVAL_TYPE_AND_OR = 0
STATUS_ENABLED = 0
STATUS_DISABLED = 1
CONDITION_TYPE_TRIGGER_VALUE = 5
CONDITION_TYPE_MAINTENANCE = 16

CONDITION_TYPE_OPE_EQUAL = 0
CONDITION_TYPE_OPE_NOT_IN = 7

def make_request_url(server):
  return "http://" + server + "/zabbix/api_jsonrpc.php"

def check_zabbix_api_response(res, url):
  if res.status_code != requests.codes.ok:
    print "Error: HTTP: %d: %s, res: %s" % (res.status_code, url, res.text)
    raise IOError

  res_json = res.json()
  if "error" in res_json:
    print "Error: ZABBIX API: %d: %s, res: %s" % (res.status_code, url, res.text)
    raise IOError
  return res_json

def get_auth_token(server):
  headers = {'content-type': 'application/json'}
  payload = {
    "auth":None,
    "method":"user.login",
    "id":1,
    "params":{"password":"zabbix", "user":"admin"},
    "jsonrpc":"2.0",
  }
  url = make_request_url(server)
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  return res_json["result"]

def host_exists(server, auth_token, host):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth":auth_token,
    "method":"host.exists",
    "id":1,
    "params":{
      "host":host,
    },
    "jsonrpc":"2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  return res_json["result"];

def host_get(server, auth_token, host):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth":auth_token,
    "method":"host.get",
    "id":1,
    "params": {
        "output": "extend",
        "filter": {"host":[host]},
    },
    "jsonrpc":"2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)

  # check the result
  num_hosts = len(res_json["result"])
  if num_hosts == 0:
    return None
  elif num_hosts != 1:
    print "The number of returned host is not one (%d)" % num_hosts
    raise AssertionError
  return res_json["result"][0]

def delete_host(server, auth_token, hostid):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth":auth_token,
    "method":"host.delete",
    "id":1,
    "params":[{"hostid":hostid}],
    "jsonrpc":"2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)

def create_proxy(server, auth_token, host):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth":auth_token,
    "method":"host.create",
    "id":1,
    "params":{
      "host":host,
      "groups": [{"groupid": "2"}],
      "interfaces": [{
        "type": 1,
        "main": 1,
        "useip": 1,
        "ip": "192.168.1.33",
        "dns": "",
        "port": "10050"
      }],
    },
    "jsonrpc":"2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)

  # check the result
  num_hosts = len(res_json["result"]["hostids"])
  if num_hosts == 0:
    return None
  elif num_hosts != 1:
    print "The number of returned host is not one (%d)" % num_hosts
    raise AssertionError
  return res_json["result"]["hostids"][0]

def create_host(server, auth_token, proxyid):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth":auth_token,
    "method":"host.create",
    "id":1,
    "params":{
      "host":test_params["host"],
      "groups": [{"groupid":group_linux_servers}],
      "interfaces": [{
        "type": 1,
        "main": 1,
        "useip": 1,
        "ip": "192.168.1.33",
        "dns": "",
        "port": "10050"
      }],
      "templates": [{"templateid": templateid_os_linux}],
      "ipmi_authtype":1,  # available
      "ipmi_password":"foo",
      "ipmi_privilege":3, # admin
      "ipmi_username":"himawari",
      "name":"Green Tea",
      "proxy_hostid":proxyid,
      "status":0, # monitored host
    },
    "jsonrpc":"2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  return res_json["result"]["hostids"][0]

def get_interface_id(target_zabbix_server, auth_token, host_id):
  headers = {'content-type': 'application/json'}
  url = make_request_url(target_zabbix_server)
  payload = {
    "auth":auth_token,
    "method":"host.get",
    "id":1,
    "params": {
        "output": "extend",
        "hostids": [host_id],
        "selectInterfaces": "extend",
    },
    "jsonrpc":"2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  return res_json["result"][0]["interfaces"].keys()[0] # key is interfaceid

def add_item(server, auth_token, hostid, interfaceid):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth":auth_token,
    "method":"item.create",
    "id":1,
    "params": {
        "name": "simple ping",
        "key_": "icmpping[]",
        "hostid": hostid,
        "type": 0,
        "value_type": 3,
        "interfaceid": interfaceid,
        "delay": 30
    },
    "jsonrpc":"2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  return res_json["result"]["itemids"][0]

def disable_item(server, auth_token, item_id):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth": auth_token,
    "method": "item.update",
    "id": 1,
    "params": {
        "itemid": item_id,
        "status": STATUS_DISABLED,
    },
    "jsonrpc": "2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  return res_json["result"]["itemids"][0]

def add_trigger(server, auth_token, hostid):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth":auth_token,
    "method":"trigger.create",
    "id":1,
    "params": {
      "description": "Context Switch over three times",
      "expression": "{%s:system.cpu.switches.last(0)}=3" % test_params["host"],
      #"dependencies": [{"triggerid": "14062"}]
    },
    "jsonrpc":"2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  return res_json["result"]["triggerids"][0]

def add_action(server, auth_token):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth":auth_token,
    "method":"action.create",
    "id":1,
    "params": {
      "name": test_params["action-name"],
      "eventsource": EVENT_SOURCE_TRIGGER,
      "evaltype": EVAL_TYPE_AND_OR,
      "status": STATUS_ENABLED,
      "esc_period": 120,
      "def_shortdata": "{TRIGGER.NAME}: {TRIGGER.STATUS}",
      "def_longdata": "{TRIGGER.NAME}: {TRIGGER.STATUS}\r\nLast value: {ITEM.LASTVALUE}\r\n\r\n{TRIGGER.URL}",

      "conditions": [{
        "conditiontype": CONDITION_TYPE_MAINTENANCE,
        "operator": CONDITION_TYPE_OPE_NOT_IN, 
      }, {
        "conditiontype": CONDITION_TYPE_TRIGGER_VALUE,
        "operator": CONDITION_TYPE_OPE_EQUAL,
        "value": "1"
      },],

      "operations": [{
        "operationtype": OPERATION_TYPE_SEND_MESSAGE,
        "esc_period": 0,
        "esc_step_from": 1,
        "esc_step_to": 0,
        "evaltype": EVAL_TYPE_AND_OR,
        "opmessage_grp": [{"usrgrpid": "7"}],
        "opmessage": {
          "default_msg": 1,
          "mediatypeid": "5" # mymail
        }
      }],
    },
    "jsonrpc":"2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  return res_json["result"]["actionids"][0]

def action_get(server, auth_token, action_name):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth": auth_token,
    "method": "action.get",
    "id": 1,
    "params": {
        "output": "extend",
        "filter": {
            "name": action_name,
        },
    },
    "jsonrpc": "2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  num_actions = len(res_json["result"])
  if num_actions == 0:
    return None
  elif num_actions != 1:
    print "The number of returned actions is not one (%d)" % num_actions
    raise AssertionError
  return res_json["result"][0]

def delete_action(server, auth_token, actionid):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth":auth_token,
    "method":"action.delete",
    "id":1,
    "params":[actionid,],
    "jsonrpc":"2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)

def action_seq(server, auth_token):

  # check if the action is already exists
  action = action_get(server, auth_token, test_params["action-name"])
  if action:
    delete_action(server, auth_token, action["actionid"])

  # add action
  action_id = add_action(target_zabbix_server, auth_token)
  print "Created action      : %s" % action_id
  return action_id

def add_user_media(server, auth_token):
  user_id = 1 # Admin
  media_type_id = 5 # mymail
  mail_addr = "zabbix@localhost"

  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth": auth_token,
    "method": "user.addmedia",
    "id": 1,
    "params": {
        "users": [{"userid": user_id},],
        "medias": {
            "mediatypeid": media_type_id,
            "sendto": mail_addr,
            "active": 0,
            "severity": 62,
            "period": "1-7,01:00-23:00"
        }
    },
    "jsonrpc": "2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  return res_json["result"]["mediaids"][0]

def user_media_seq(server, auth_token):
  media_id = add_user_media(server, auth_token)
  print "Added user media    : %s" % media_id
  return media_id

  delete_maintenance_if_exists(target_zabbix_server, auth_token, test_params["maintenance-name"])

def get_maintenance(server, auth_token, host_id):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth": auth_token,
    "method": "maintenance.get",
    "id": 1,
      "params": {
        "output": "extend",
        #"selectGroups": "extend",
        #"hostids": host_id,
    },
    "jsonrpc": "2.0",
  },
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  num_maintenances = len(res_json)
  if num_maintenances == 0:
    return None
  elif num_maintenances != 1:
    print "The number of returned maintenances is not one (%d)" % num_maintenances
    raise AssertionError

  result = res_json[0]["result"]
  num_results = len(result)
  if num_results == 0:
    return None
  elif num_results != 1:
    print "The number of returned results is not one (%d)" % num_results
  return result[0]

def delete_maintenance(server, auth_token, maintenance_id):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth": auth_token,
    "method": "maintenance.delete",
    "id": 1,
    "params": [maintenance_id],
    "jsonrpc": "2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)


def add_maintenace(server, auth_token, host_id):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth": auth_token,
    "method": "maintenance.create",
    "id": 1,
    "params": {
        "name": test_params["maintenance-name"],
        "active_since": int(time.time()),
        "active_till": int(time.time()+3600),
        "hostids": [host_id],
        "timeperiods": [
            {
                "timeperiod_type": 3,
                "every": 1,
                "dayofweek": 64,
                "start_time": 64800,
                "period": 3600
            }
        ]
    },
    "jsonrpc": "2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  return res_json["result"]["maintenanceids"][0]

def show_events(server, auth_token):
  headers = {'content-type': 'application/json'}
  url = make_request_url(server)
  payload = {
    "auth": auth_token,
    "method": "event.get",
    "id": 1,
    "params": {
        "output": "extend",
        "select_acknowledges": "extend",
        "sortfield": "eventid",
        "sortorder": "DESC"
    },
    "jsonrpc": "2.0",
  }
  res = requests.post(url, data=json.dumps(payload), headers=headers)
  res_json = check_zabbix_api_response(res, url)
  print res_json


# ----------------------------------------------------------------------------
# main code
# ----------------------------------------------------------------------------
only_action = False
only_user_media = False
set_host_maintenance = False
only_show_events = False
for arg in sys.argv:
  if arg == "--only-action":
    print "mode: Only Action"
    only_action = True
  elif arg == "--only-user-media":
    print "mode: Only user media"
    only_user_media = True
  elif arg == "--set-maintenance":
    print "set maintenance"
    set_host_maintenance = True
  elif arg == "--only-show-events":
    print "mode: Only show events"
    only_show_events = True

target_zabbix_server = "localhost"
print "Target Zabbix Server: %s" % target_zabbix_server
auth_token = get_auth_token(target_zabbix_server)
print "AuthToken           : %s" % auth_token

if only_action:
  action_seq(target_zabbix_server, auth_token)
  sys.exit(0)

if only_user_media:
  user_media_seq(target_zabbix_server, auth_token)
  sys.exit(0)

if only_show_events:
  show_events(target_zabbix_server, auth_token)
  sys.exit(0)

# check if the host exists
host = host_get(target_zabbix_server, auth_token, test_params["host"])
print "Check test host     : %s" % (not not host)
if host:
  delete_host(target_zabbix_server, auth_token, host["hostid"])
  print "Deleted host        : %s" % host["host"]

# create host for proxy if it doesnot exist
proxy = host_get(target_zabbix_server, auth_token, test_params["proxy-host"])
print "Check test proxy    : %s" % (not not proxy)
if proxy:
  proxyid = proxy["hostid"]
else:
  proxyid = create_proxy(target_zabbix_server, auth_token, test_params["proxy-host"])
  print "Created proxy       : %s" % host["proxy-host"]

# create host
host_id = create_host(target_zabbix_server, auth_token, proxyid)
print "Created host        : %s, host ID: %s" % (test_params["host"], host_id)

# get interface id
interface_id = get_interface_id(target_zabbix_server, auth_token, host_id)
print "Interface ID        : %s" % interface_id

# add item
item_id = add_item(target_zabbix_server, auth_token, host_id, interface_id)

# disable item
item_id = disable_item(target_zabbix_server, auth_token, item_id)
print "Disabled item       : %s" % item_id

# add trigger
trigger_id = add_trigger(target_zabbix_server, auth_token, host_id)
print "Create trigger      : %s" % trigger_id

# add action
action_id = action_seq(target_zabbix_server, auth_token)

# add media
media_id = user_media_seq(target_zabbix_server, auth_token)

# set maintenance
if set_host_maintenance:
  maintenance = get_maintenance(target_zabbix_server, auth_token, host_id)
  if maintenance:
    delete_maintenance(target_zabbix_server, auth_token, maintenance["maintenanceid"])
  maintenance_id = add_maintenace(target_zabbix_server, auth_token, host_id)
  print "Maintenace:          : %s" % maintenance_id
