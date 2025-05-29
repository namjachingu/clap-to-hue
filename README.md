# Introduction
This project provides an interface to the Philips Hue API, enabling communication with Philips Hue lightbulbs. It includes an HTTP client for retrieving information about the lightbulbs and switching them off.

# Building steps
1. Run your favorite compiler to create an executable. E.g with clang:
   1.1 clang++ -std=c++20 -o philipshueclient main.cpp CurlClient.cpp Lights.cpp -lcurl -lfmt
   1.2 ./philipshueclient

or

2. Build with CMake, following these steps:
   2.1 cmake .
   2.2 make
   2.3 ./philipshueclient


# Certificate
The Philips Hue Bridge uses certificates to secure communication over HTTPS. Older Hue Bridges may still use self-signed certificates, while newer bridges utilize certificates signed by Signify’s private CA for Hue Bridges. Proper validation of these certificates is essential for secure communication.

The Hue API can be accessed over HTTP on port 80 and HTTPS on port 443. HTTP support might be deprecated in the future due to security concerns, hence, this project focuses on HTTPS. In order to establish a secure HTTPS connection, the client must validate the Hue Bridge certificate against the Signify private CA Certificate for Hue Bridges. This can be achieved by adding the CA certificate to the HTTP client’s trusted key store or by providing it as an input parameter during the HTTPS connection setup. The CA certificate is provided in PEM format, which can be located on Philip HUe's API pages.

Additionally, the client must verify that the certificate’s Common Name (CN) matches the bridge ID of the target Hue Bridge. This can be done by using the bridge ID as the hostname and resolving it to the IP address.

# CURL examples
Below are examples demonstrating how to interact with the Philips Hue API using CURL:

1. Retrieve the list of lights using HTTP:
`curl http://<ipaddress>/api/<username>/lights`

2. Retrive the list of lights using HTTPS:
`curl --cacert huebridge_cacert.pem --resolve "<bridgeid>:443:<ipaddress>" https://<bridgeid>/api/<username>/lights`

In both examples, replace <ipaddress>, <username>, and <bridgeid> with the corresponding values for your Philips Hue Bridge.

<ipaddress>: The IP address of the Hue Bridge.
<username>: The API username generated for accessing the Hue Bridge.
<bridgeid>: The unique identifier of the Hue Bridge.

For HTTPS connections, ensure that the huebridge_cacert.pem file contains the trusted CA certificate for the Hue Bridge. The --resolve option maps the bridge ID to its IP address and port, enabling secure communication

# Arduino integration
This project utilizes an Arduino Uno for detecting sound levels and controlling the Philips Hue lights accordingly. Since the Arduino Uno lacks built-in Wi-Fi capabilities, the following workarounds are suggested:

1. Execute the Python script `run.py` on a computer, Raspberry Pi, or another device capable of network communication.
2. Integrate an external Wi-Fi module with the Arduino (e.g., ESP8266 module). This feature is currently work in progress.


### Circuit Configuration
Set up the Arduino circuit for sound detection:

  - Connect the audio signal pin (`AUD`) to analog input `A0`.
  - Connect `GND` to the ground pin (`GND`).
  - Connect `VCC` to the 3.3V power pin (3.3V provides optimal results).


Configure the sound detection thresholds for triggering the LED:

1. Open the serial communication port in the Arduino IDE (accessible via the magnifying glass icon in the top-right corner).
2. Perform experiments with various sound inputs (e.g., clapping, snapping, blowing, door slamming, knocking) to observe the resting noise levels and the thresholds for loud noises.
3. Modify the `if` statement in the code based on your findings to adjust the sensitivity.
