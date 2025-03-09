# Starlink-tracker

To run the application:
```
python3 fetchApi.py --api_key <api_key> 
```

```
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

(Ubuntu) If you have a problem with:
```
 UserWarning: FigureCanvasAgg is non-interactive, and thus cannot be shown
  plt.show()
```
Use: `sudo apt install --reinstall qtbase5-dev qtwayland5 libqt5gui5 libqt5widgets5 libxcb-xinerama0 libxcb-cursor0`
