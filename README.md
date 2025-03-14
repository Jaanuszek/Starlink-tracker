# Starlink-tracker

## C++ Program

For now, the program should be run in `Release` configuration on `x64` platform

### Requirements
* Visual Studio 2022 with C++ packages
* OpenSSL

### Libcurl build
In case of libcurl erros, download it and build it yourself.

Download it from here [curl github](https://github.com/curl/curl) and then unzip/untar it. Then use this command (make sure you have openssl!) and change the paths to match the location of the files on your computer.


```
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCURL_STATICLIB=ON -DCURL_USE_LIBPSL=OFF -DCURL_USE_OPENSSL=ON -DOPENSSL_ROOT_DIR="C:\Program Files\OpenSSL-Win64" -DOPENSSL_INCLUDE_DIR="PATH_TO_OPENSSL_INCLUDE_DIR" -DOPENSSL_CRYPTO_LIBRARY="PATH_TO_OPENSSL_LIBCRYPTO_LIB" -DOPENSSL_SSL_LIBRARY="PATH_TO_OPENSSL_LIBSSL_LIB"
```

For example:

```
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCURL_STATICLIB=ON -DCURL_USE_LIBPSL=OFF -DCURL_USE_OPENSSL=ON -DOPENSSL_ROOT_DIR="C:\Program Files\OpenSSL-Win64" -DOPENSSL_INCLUDE_DIR="C:\Program Files\OpenSSL-Win64\include" -DOPENSSL_CRYPTO_LIBRARY="C:\Program Files\OpenSSL-Win64\lib\VC\x64\MD\libcrypto.lib" -DOPENSSL_SSL_LIBRARY="C:\Program Files\OpenSSL-Win64\lib\VC\x64\MD\libssl.lib"
```

Then go to `build` dir and execute `cmake --build . --config Release`.

Finally, copy `build/lib/Release/libcurl.lib` to `StarlinkTracker/StarlinkTracker/external/curl/lib/x64/`.

## Python program
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
