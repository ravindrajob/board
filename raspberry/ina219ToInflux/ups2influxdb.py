# Author: Ravindra JOB
#  SiteWeb: ravindra-job.com
# LastUpdate : 24/10/2020

# IMPORT THE LIBERARY.
from influxdb_client import InfluxDBClient, Point
from ina219 import INA219
from ina219 import DeviceRangeError
SHUNT_OHMS = 0.05
mv=float()
current=float()
power=float()
shunt=float()

#from influxdb_client import InfluxDBClient, Point
username = 'ups'
password = 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
database = 'ups'
retention_policy = 'autogen'
bucket = f'{database}/{retention_policy}'
client = InfluxDBClient(url='https://database.ravindra-job', token=f'{username}:{password}', org='-')
print('*** Write Points ***')
write_api = client.write_api()

def read():
    """Define method to read information from coulometer."""
    ina = INA219(SHUNT_OHMS)
    ina.configure()
    current = "%.3f" % ina.voltage()
    power = "%.3f" % ina.power()
    shunt = "%.3f" % ina.shunt_voltage()

    point = Point("rpi").tag("host", "rpi4").field("current", current)
    point2 = Point("rpi").tag("host", "rpi4").field("power", power)
    point3 = Point("rpi").tag("host", "rpi4").field("shunt", shunt)

    print(point.to_line_protocol())
    print(point2.to_line_protocol())
    print(point3.to_line_protocol())
    write_api.write(bucket=bucket, record=point3)
    write_api.write(bucket=bucket, record=point2)
    write_api.write(bucket=bucket, record=point)
    write_api.__del__()

    print("Bus Voltage: %.3f V" % ina.voltage())
    try:
        print("Bus Current: %.3f mA" % ina.current())
        print("Power: %.3f mW" % ina.power())
        print("Shunt voltage: %.3f mV" % ina.shunt_voltage())
    except DeviceRangeError as e:
        print(e)
if __name__ == "__main__":
    read()

print('*** Query Points ***')
client.close()
