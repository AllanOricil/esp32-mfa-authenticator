# ESP32 MFA TOTP Generator

A pet project that displays MFA TOTP codes. The goal is to allow me to get TOTPs for short living sessions without interacting with my phone. At the moment every time I need a new TOTP, I have to:

- find my phone
- hope it is charged
- if it is not, then charge it -> also have to wait a few minutes until it is fully operational to continue
- if it is, unlock it
- find a Virtual MFA App
- unlock it
- find the TOTP for the service I need

My plan is to place this MFA device under my monitor and just look at it when I need to get a new TOTP. It is going to generate TOTPs for the services I use the most, and have the shortest non configurable session times.

## Secrets

Secrets are stored in a file called `secrets.txt`, located in the root of an SD card. It must contain the following format:

````bash
service_id,encoded_base_32_secret
````

for example

````bash
aws,DSAJDHHAHASAUDOASNOTREALOADAKLDASAJFPOAIDONTEVENTRYOASFAIPO
````

