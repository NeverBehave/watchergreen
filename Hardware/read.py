from threading import Thread
from urllib2 import Request, urlopen, HTTPError, URLError
import urllib2
import json
import serial

endpoint = ''
p = sys.argv[1]
ser = serial.Serial(port=p, baudrate=9600)

print "Serial Connect!"
str = ''

def post(data):
  data = data.split(':')
  data = {
      "light": int(data[2]),
      "temperature" : int(data[0]),
      "humility " : int(data[1]),
      "location": { "latitude": "42.7284", "longtitude": "-73.6918" }
  }
  opener = urllib2.build_opener(urllib2.HTTPHandler)
  request = urllib2.Request(endpoint, data=json.dumps(data))
  print data
  request.add_header('Content-Type', 'application/json')
  request.add_header('User-Agent', 'curl/7.54.0')
  request.get_method = lambda: 'PUT'
  url = opener.open(request)

while True:
  for line in ser.read():
    if not line == '\n':
      str += line
    else:
      print str
      thread = Thread(target=post, args=[str])
      thread.start()
      str = ''
