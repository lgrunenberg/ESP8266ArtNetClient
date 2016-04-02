# ESP8266ArtNetClient

## What is this?

This aims to be a fully integrated wireless Art-Net compatible node that can control RGB LEDs or WS2812 LED stripes by DMX channels.

## TODO

* general code cleanup
  + offload Art-Net package decoding to dedicated function
* add cyclic reconnection attempts
* add power saving modes

## How to use

Install the ESP8266 Arduino Package and the Adafruit NeoPixel Library using Arduinos package manager. Make sure to edit the SSID and password accordingly. Compile and upload the sketch to your ESP8266 and connect a WS2812 stripe to GPIO2. 

## Warning!

This is Pre-Alpha stage and has not been thoroughly tested. This repository is mainly for internal development purposes! Do not use!