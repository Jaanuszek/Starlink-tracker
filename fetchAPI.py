# import json
# import requests
# import time 
# from skyfield.api import load, EarthSatellite
# from datetime import datetime

# print("SIEMA ENIU")

# response = requests.get("https://api.spacexdata.com/v4/starlink")

# response_json = response.json()
# # response_dict = json.loads(response.text)
# # print(response.text)
# # print(response_dict)
# print(response.status_code)
# if(response.status_code == 200):
#     print("Succesfully fetched")
#     # print(response_json)
#     # if isinstance(response_json, list) and len(response_json) > 0:
#     #     last_update = response_json[0].get('updated_at', 'brak daty aktualizacji')
#     #     last_update = response_json[0].get('updated_at', 'brak daty aktualizacji')
#     #     print(f'Ostatnia aktualizacja: {last_update}')
#     # else:
#     #     print("Brak danych w odpowiedzi.")
#     for i in response_json:
#         print(i["spaceTrack"]["TLE_LINE0"])
#         print(i["spaceTrack"]["TLE_LINE1"])
#         print(i["spaceTrack"]["TLE_LINE2"])
#         tle_name=i["spaceTrack"]["TLE_LINE0"]
#         tle_line1=i["spaceTrack"]["TLE_LINE1"]
#         tle_line2=i["spaceTrack"]["TLE_LINE2"]
#         starlinkInfo=EarthSatellite(tle_line1, tle_line2, tle_name, load.timescale())
#         t = load.timescale().now()
#         geocentric = starlinkInfo.at(t)
#         # lat, lon, alt = geocentric.subpoint()
#         subpoint = geocentric.subpoint()
#         lat = subpoint.latitude.degrees
#         lon = subpoint.longitude.degrees
#         alt = subpoint.elevation.km
#         print(f"Szerokość: {lat}, Długość: {lon}, Wysokość: {alt} km")
#         # print(i)
#         # print("lognitude: ", i["longitude"])
#         # print("latitude: ", i["latitude"])
#         time.sleep(1)

#     # for i in response_dict:
#         # print("key: ", i, "val: ", response_dict[i])
# elif(response.status_code == 400):
#     print("Response error 400")
# else:
#     print("Response error 500")

import requests
import folium
import time

API_KEY = ""

SAT_ID = 56144

# Twoje współrzędne geograficzne (opcjonalnie dla predykcji przelotów)
LAT = 52.2298  # szerokość geograficzna (np. Warszawa)
LON = 21.0122  # długość geograficzna
ALT = 100      # wysokość w metrach

m = folium.Map(location=[0, 0], zoom_start = 2)
path = []

def getStarlikPosiotion():
    # Endpoint API do pobrania pozycji satelity w czasie rzeczywistym
    url = f"https://api.n2yo.com/rest/v1/satellite/positions/{SAT_ID}/{LAT}/{LON}/{ALT}/1001/?apiKey={API_KEY}"

    # Wysyłanie zapytania do API
    response = requests.get(url)

    # Sprawdzanie odpowiedzi
    if response.status_code == 200:
        data = response.json()
        # print("Pozycja satelity:")
        # print(f"Szerokość: {data['positions'][0]['satlatitude']}")
        # print(f"Długość: {data['positions'][0]['satlongitude']}")
        # print(f"Wysokość: {data['positions'][0]['sataltitude']} km")
        position = data['positions']
        # lat, lon = position['satlatitude'], position['satlongitude']
        path.extend([(pos['satlatitude'], pos['satlongitude']) for pos in position])
        # path.append((lat,lon))
        return [(pos['satlatitude'], pos['satlongitude']) for pos in position]
    else:
        print("Błąd:", response.status_code, response.text)
        return []

for _ in range(1):
    pathReceived = getStarlikPosiotion()
    # if pathReceived:
    for (lat, lon) in path:
        folium.Marker(location=[lat, lon], popup="Starlink").add_to(m)
    time.sleep(5)

folium.PolyLine(path, color = "red", weight = 2.5, opacity=1).add_to(m)

m.save("starlink_map.html")
