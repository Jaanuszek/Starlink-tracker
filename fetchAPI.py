import argparse
import sys
import requests
import time
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation

parser = argparse.ArgumentParser(
    prog = 'Starlink-tracker',
    description= 'Program for tracking starlinks xd'
    )
parser.add_argument(
    '--api_key', '-k', 
    dest='apiKey',
    type=str, 
    help='Add api key',
    required=True
    )
args = parser.parse_args()

if (len(args.apiKey) != 25):
    print("An api key of incorrect length was entered")
    exit(1)

API_KEY = args.apiKey

SAT_ID = 56144

LAT = 52.2298
LON = 21.0122
ALT = 100

path = []

def getStarlikPosiotion():
    url = f"https://api.n2yo.com/rest/v1/satellite/positions/{SAT_ID}/{LAT}/{LON}/{ALT}/1/?apiKey={API_KEY}"

    response = requests.get(url)

    if response.status_code == 200:
        data = response.json()
        SatelitePosition = data['positions']
        path.extend([(pos['satlatitude'], pos['satlongitude']) for pos in SatelitePosition])
        return [(pos['satlatitude'], pos['satlongitude']) for pos in SatelitePosition]
    else:
        print("Błąd:", response.status_code, response.text)
        return []

def updateMap(frame):
    global posList

    pathReceived = getStarlikPosiotion()
    lat, lon = zip(*pathReceived)

    lons, lats = map(lon, lat)
    # Joining two lists lons and lats into a list of (lons,lats) tuples
    posList.extend(zip(lons, lats))

    ax.clear()
    map.drawcoastlines(linewidth=0.5)
    map.drawmapboundary(fill_color='aqua')
    map.fillcontinents(color='coral', lake_color='aqua')
    map.drawcountries(linewidth=0.25)
    
    for x,y in posList:
        map.plot(x,y, marker='o', color='r')

    plt.title("siemaEniu")


fig, ax = plt.subplots(figsize=(10,6))
map = Basemap(projection='mill', ax=ax)
map.drawcoastlines(linewidth=0.5)
map.drawmapboundary(fill_color='aqua')
map.fillcontinents(color='coral', lake_color='aqua')
map.drawcountries(linewidth=0.25)

posList = []

# pathReceived = getStarlikPosiotion()
# lat, lon = zip(*pathReceived)
# # for point in pathReceived:
# #     lat = point[1]
# #     lon = point[0]
# lons, lats = map(lon, lat)
# map.plot(lons, lats, marker = 'o', color='r')


# plt.title("siemaEniu")
ani = FuncAnimation(fig, updateMap, interval=5000, cache_frame_data=False)
plt.show()
