STM32F7-ETH-LAN8720A-lwIP-MQTT
==============================

MQTT demo project for [32F746GDISCOVERY](http://www.st.com/en/evaluation-tools/32f746gdiscovery.html) (also known as
*STM32F746G-DISCO*) and [NUCLEO-F767ZI](https://www.st.com/en/evaluation-tools/nucleo-f767zi.html) boards. It features
following components:
- *distortos*,
- *ST's* *STM32F7 HAL* library,
- ETH driver for *Microchip LAN8720A* PHY,
- *lwIP*.

Configuration & building
------------------------

When you want to use *32F746GDISCOVERY* board:

    $ mkdir output
    $ cd output
    $ cmake -C../configurations/ST_32F746GDISCOVERY/distortosConfiguration.cmake .. -GNinja
    $ ninja

When you want to use *NUCLEO-F767ZI* board:

    $ mkdir output
    $ cd output
    $ cmake -C../configurations/ST_NUCLEO-F767ZI/distortosConfiguration.cmake .. -GNinja
    $ ninja

For more in-depth instructions see `distortos/README.md`.

MQTT
----

Once the device connects to the network, it tries to connect to MQTT broker at `broker.hivemq.com`. When you subscribe
to the topic `distortos/#`, you should see the messages with device's online status (`1` is sent when the device
connects, `0` as a MQTT client's will once the connection is lost) and changes of button state.

With *32F746GDISCOVERY* board:

```
$ mosquitto_sub -h broker.hivemq.com -t "distortos/#" -v
distortos/0.7.0/ST,32F746GDISCOVERY/online 1
distortos/0.7.0/ST,32F746GDISCOVERY/buttons/0/state 0
distortos/0.7.0/ST,32F746GDISCOVERY/buttons/0/state 1
distortos/0.7.0/ST,32F746GDISCOVERY/buttons/0/state 0
```

With *NUCLEO-F767ZI* board:

```
$ mosquitto_sub -h broker.hivemq.com -t "distortos/#" -v
distortos/0.7.0/ST,NUCLEO-F767ZI/online 1
distortos/0.7.0/ST,NUCLEO-F767ZI/buttons/0/state 0
distortos/0.7.0/ST,NUCLEO-F767ZI/buttons/0/state 1
distortos/0.7.0/ST,NUCLEO-F767ZI/buttons/0/state 0
```

The application also allows you to control on-board LEDs via MQTT.

*32F746GDISCOVERY* board has just one LED available:

```
# set the LED on
$ mosquitto_pub -h broker.hivemq.com -t "distortos/0.7.0/ST,32F746GDISCOVERY/leds/0/state" -m "1"
# set the LED off
$ mosquitto_pub -h broker.hivemq.com -t "distortos/0.7.0/ST,32F746GDISCOVERY/leds/0/state" -m "0"
```

On *NUCLEO-F767ZI* board there are 3 LEDs:

```
# set green LED on
$ mosquitto_pub -h broker.hivemq.com -t "distortos/0.7.0/ST,NUCLEO-F767ZI/leds/0/state" -m "1"
# set blue LED on
$ mosquitto_pub -h broker.hivemq.com -t "distortos/0.7.0/ST,NUCLEO-F767ZI/leds/1/state" -m "1"
# set red LED on
$ mosquitto_pub -h broker.hivemq.com -t "distortos/0.7.0/ST,NUCLEO-F767ZI/leds/2/state" -m "1"
# set green LED off
$ mosquitto_pub -h broker.hivemq.com -t "distortos/0.7.0/ST,NUCLEO-F767ZI/leds/0/state" -m "0"
# set blue LED off
$ mosquitto_pub -h broker.hivemq.com -t "distortos/0.7.0/ST,NUCLEO-F767ZI/leds/1/state" -m "0"
# set red LED off
$ mosquitto_pub -h broker.hivemq.com -t "distortos/0.7.0/ST,NUCLEO-F767ZI/leds/2/state" -m "0"
```

Debug output
------------

ST-Link V2-1 has a virtual COM port which is used for debug output from the application. The stream uses typical
parameters: 115200 bps, 8N1. Below you will find the example of the output from *32F746GDISCOVERY* board.

```
[2020-04-18 11:49:45] Started ST,32F746GDISCOVERY board
[2020-04-18 11:49:46] dns_init: initializing
[2020-04-18 11:49:47] netif: added interface st IP addr 0.0.0.0 netmask 0.0.0.0 gw 0.0.0.0
[2020-04-18 11:49:47] netif: setting default interface st
[2020-04-18 11:49:47] netifStatusCallback: netif = st0, link = down, status = up
[2020-04-18 11:49:47] dhcp_start(netif=0x2004ff94) st0
[2020-04-18 11:49:47] dhcp_start(): mallocing new DHCP client
[2020-04-18 11:49:47] dhcp_start(): allocated dhcpdhcp_start(): starting DHCP configuration
[2020-04-18 11:49:47] lwip_getaddrinfo() failed, ret = 202
[2020-04-18 11:49:47] dns_tmr: dns_check_entries
[2020-04-18 11:49:48] dns_tmr: dns_check_entries
[2020-04-18 11:49:49] dhcp_discover()
[2020-04-18 11:49:49] transaction id xid(5851f42d)
[2020-04-18 11:49:49] dhcp_discover: making request
[2020-04-18 11:49:49] dhcp_discover: sendto(DISCOVER, IP_ADDR_BROADCAST, LWIP_IANA_PORT_DHCP_SERVER)
[2020-04-18 11:49:49] dhcp_discover: deleting()
[2020-04-18 11:49:49] dhcp_discover: SELECTING
[2020-04-18 11:49:49] dhcp_discover(): set request timeout 2000 msecs
[2020-04-18 11:49:49] netifLinkCallback: netif = st0, link = up
[2020-04-18 11:49:49] dns_tmr: dns_check_entries
[2020-04-18 11:49:50] dns_tmr: dns_check_entries
[2020-04-18 11:49:50] dhcp_fine_tmr(): request timeout
[2020-04-18 11:49:50] dhcp_timeout()
[2020-04-18 11:49:50] dhcp_timeout(): restarting discovery
[2020-04-18 11:49:50] dhcp_discover()
[2020-04-18 11:49:50] transaction id xid(5851f42d)
[2020-04-18 11:49:50] dhcp_discover: making request
[2020-04-18 11:49:50] dhcp_discover: sendto(DISCOVER, IP_ADDR_BROADCAST, LWIP_IANA_PORT_DHCP_SERVER)
[2020-04-18 11:49:50] dhcp_discover: deleting()
[2020-04-18 11:49:50] dhcp_discover: SELECTING
[2020-04-18 11:49:50] dhcp_discover(): set request timeout 4000 msecs
[2020-04-18 11:49:51] dns_tmr: dns_check_entries
[2020-04-18 11:49:52] lwip_getaddrinfo() failed, ret = 202
[2020-04-18 11:49:52] dns_tmr: dns_check_entries
[2020-04-18 11:49:52] dhcp_recv(pbuf = 0x2000d680) from DHCP server 192.168.1.1 port 67
[2020-04-18 11:49:52] pbuf->len = 548
[2020-04-18 11:49:52] pbuf->tot_len = 548
[2020-04-18 11:49:52] skipping option 125 in options
[2020-04-18 11:49:52] searching DHCP_OPTION_MESSAGE_TYPE
[2020-04-18 11:49:52] DHCP_OFFER received in DHCP_STATE_SELECTING state
[2020-04-18 11:49:52] dhcp_handle_offer(netif=0x2004ff94) st0
[2020-04-18 11:49:52] dhcp_handle_offer(): server 0x0101a8c0
[2020-04-18 11:49:52] dhcp_handle_offer(): offer for 0x0601a8c0
[2020-04-18 11:49:52] dhcp_select(netif=0x2004ff94) st0
[2020-04-18 11:49:52] transaction id xid(5851f42d)
[2020-04-18 11:49:52] dhcp_select: REQUESTING
[2020-04-18 11:49:52] dhcp_select(): set request timeout 2000 msecs
[2020-04-18 11:49:52] dhcp_recv(pbuf = 0x2000d680) from DHCP server 192.168.1.1 port 67
[2020-04-18 11:49:52] pbuf->len = 548
[2020-04-18 11:49:52] pbuf->tot_len = 548
[2020-04-18 11:49:52] skipping option 125 in options
[2020-04-18 11:49:52] searching DHCP_OPTION_MESSAGE_TYPE
[2020-04-18 11:49:52] DHCP_ACK received
[2020-04-18 11:49:52] dhcp_check(netif=0x2004ff94) st
[2020-04-18 11:49:53] dns_tmr: dns_check_entries
...
[2020-04-18 11:49:56] dns_tmr: dns_check_entries
[2020-04-18 11:49:57] dns_enqueue: "broker.hivemq.com": use DNS entry 0
[2020-04-18 11:49:57] dns_enqueue: "broker.hivemq.com": use DNS pcb 0
[2020-04-18 11:49:57] dns_send: dns_servers[0] "broker.hivemq.com": request
[2020-04-18 11:49:57] sending DNS request ID 12585 for name "broker.hivemq.com" to server 0
[2020-04-18 11:49:57] dns_send returned error: Routing problem.
[2020-04-18 11:49:57] dns_tmr: dns_check_entries
[2020-04-18 11:49:57] dns_send: dns_servers[0] "broker.hivemq.com": request
[2020-04-18 11:49:57] sending DNS request ID 12585 for name "broker.hivemq.com" to server 0
[2020-04-18 11:49:57] dns_send returned error: Routing problem.
[2020-04-18 11:49:58] dns_tmr: dns_check_entries
[2020-04-18 11:49:58] dns_send: dns_servers[0] "broker.hivemq.com": request
[2020-04-18 11:49:58] sending DNS request ID 12585 for name "broker.hivemq.com" to server 0
[2020-04-18 11:49:58] dns_send returned error: Routing problem.
[2020-04-18 11:49:59] dns_tmr: dns_check_entries
[2020-04-18 11:49:59] dhcp_bind(netif=0x2004ff94) st0
[2020-04-18 11:49:59] dhcp_bind(): t0 renewal timer 86400 secs
[2020-04-18 11:49:59] dhcp_bind(): set request timeout 86400000 msecs
[2020-04-18 11:49:59] dhcp_bind(): t1 renewal timer 43200 secs
[2020-04-18 11:49:59] dhcp_bind(): set request timeout 43200000 msecs
[2020-04-18 11:49:59] dhcp_bind(): t2 rebind timer 75600 secs
[2020-04-18 11:49:59] dhcp_bind(): set request timeout 75600000 msecs
[2020-04-18 11:49:59] dhcp_bind(): IP: 0x0601a8c0 SN: 0x00ffffff GW: 0x0101a8c0
[2020-04-18 11:49:59] netif: netmask of interface st set to 255.255.255.0
[2020-04-18 11:49:59] netif: GW address of interface st set to 192.168.1.1
[2020-04-18 11:49:59] netif_set_ipaddr: netif address being changed
[2020-04-18 11:49:59] netifStatusCallback: netif = st0, link = up, status = up
[2020-04-18 11:49:59]   ip4 = 192.168.1.6
[2020-04-18 11:49:59]   gateway = 192.168.1.1
[2020-04-18 11:49:59]   netmask = 255.255.255.0
[2020-04-18 11:50:00] dns_tmr: dns_check_entries
[2020-04-18 11:50:00] dns_send: dns_servers[0] "broker.hivemq.com": request
[2020-04-18 11:50:00] sending DNS request ID 12585 for name "broker.hivemq.com" to server 0
[2020-04-18 11:50:00] dns_recv: "broker.hivemq.com": response = 52.28.220.48
[2020-04-18 11:50:00] broker.hivemq.com is 52.28.220.48
[2020-04-18 11:50:00] MQTT client ID is "ST,32F746GDISCOVERY-004400203035510239363937"
[2020-04-18 11:50:00] mqtt_client_connect: Connecting to host: 52.28.220.48 at port:1883
[2020-04-18 11:50:00] Connecting to MQTT broker...
[2020-04-18 11:50:00] mqtt_tcp_connect_cb: TCP connection established to server
[2020-04-18 11:50:00] mqtt_output_send: tcp_sndbuf: 5840 bytes, ringbuf_linear_available: 105, get 0,
                      put 105
[2020-04-18 11:50:00] mqtt_parse_incoming: Remaining length after fixed header: 2
[2020-04-18 11:50:00] mqtt_parse_incoming: msg_idx: 4, cpy_len: 2, remaining 0
[2020-04-18 11:50:00] mqtt_message_received: Connect response code 0
[2020-04-18 11:50:00] mqttClientConnectionCallback: status = 0
[2020-04-18 11:50:01] dns_tmr: dns_check_entries
...
[2020-04-18 11:50:05] dns_tmr: dns_check_entries
[2020-04-18 11:50:05] mqtt_publish: Publish with payload length 1 to topic
                      "distortos/0.7.0/ST,32F746GDISCOVERY/online"
[2020-04-18 11:50:05] mqtt_output_send: tcp_sndbuf: 5840 bytes, ringbuf_linear_available: 47, get 105,
                      put 152
[2020-04-18 11:50:05] mqtt_sub_unsub: Client (un)subscribe to topic
                      "distortos/0.7.0/ST,32F746GDISCOVERY/leds/+/state", id: 1
[2020-04-18 11:50:05] mqtt_output_send: tcp_sndbuf: 5793 bytes, ringbuf_linear_available: 55, get 152,
                      put 207
[2020-04-18 11:50:05] mqtt_publish: Publish with payload length 1 to topic
                      "distortos/0.7.0/ST,32F746GDISCOVERY/buttons/0/state"
[2020-04-18 11:50:05] mqtt_output_send: tcp_sndbuf: 5738 bytes, ringbuf_linear_available: 49, get 207,
                      put 7
[2020-04-18 11:50:05] mqtt_tcp_sent_cb: Calling QoS 0 publish complete callback
[2020-04-18 11:50:05] mqttRequestCallback: error = 0
[2020-04-18 11:50:05] mqtt_tcp_sent_cb: Calling QoS 0 publish complete callback
[2020-04-18 11:50:05] mqttRequestCallback: error = 0
[2020-04-18 11:50:05] mqtt_parse_incoming: Remaining length after fixed header: 3
[2020-04-18 11:50:05] mqtt_parse_incoming: msg_idx: 5, cpy_len: 3, remaining 0
[2020-04-18 11:50:05] mqtt_message_received: SUBACK response with id 1
[2020-04-18 11:50:05] mqttRequestCallback: error = 0
[2020-04-18 11:50:06] dns_tmr: dns_check_entries
[2020-04-18 11:50:07] dns_tmr: dns_check_entries
[2020-04-18 11:50:07] mqtt_publish: Publish with payload length 1 to topic
                      "distortos/0.7.0/ST,32F746GDISCOVERY/buttons/0/state"
[2020-04-18 11:50:07] mqtt_output_send: tcp_sndbuf: 5840 bytes, ringbuf_linear_available: 56, get 7,
                      put 63
[2020-04-18 11:50:07] mqtt_tcp_sent_cb: Calling QoS 0 publish complete callback
[2020-04-18 11:50:07] mqttRequestCallback: error = 0
[2020-04-18 11:50:07] mqtt_publish: Publish with payload length 1 to topic
                      "distortos/0.7.0/ST,32F746GDISCOVERY/buttons/0/state"
[2020-04-18 11:50:07] mqtt_output_send: tcp_sndbuf: 5840 bytes, ringbuf_linear_available: 56, get 63,
                      put 119
[2020-04-18 11:50:07] mqtt_tcp_sent_cb: Calling QoS 0 publish complete callback
[2020-04-18 11:50:07] mqttRequestCallback: error = 0
[2020-04-18 11:50:08] dns_tmr: dns_check_entries
...
[2020-04-18 11:50:12] dns_tmr: dns_check_entries
[2020-04-18 11:50:12] mqtt_parse_incoming: Remaining length after fixed header: 51
[2020-04-18 11:50:12] mqtt_parse_incoming: msg_idx: 53, cpy_len: 51, remaining 0
[2020-04-18 11:50:12] mqtt_incomming_publish: Received message with QoS 0 at topic:
                      distortos/0.7.0/ST,32F746GDISCOVERY/leds/0/state, payload length 1
[2020-04-18 11:50:12] mqttIncomingPublishCallback: topic =
                      "distortos/0.7.0/ST,32F746GDISCOVERY/leds/0/state", total length = 1
[2020-04-18 11:50:12] mqttIncomingDataCallback: length = 1, flags = 1
[2020-04-18 11:50:13] dns_tmr: dns_check_entries
[2020-04-18 11:50:14] dns_tmr: dns_check_entries
[2020-04-18 11:50:15] dns_tmr: dns_check_entries
[2020-04-18 11:50:15] mqtt_parse_incoming: Remaining length after fixed header: 51
[2020-04-18 11:50:15] mqtt_parse_incoming: msg_idx: 53, cpy_len: 51, remaining 0
[2020-04-18 11:50:15] mqtt_incomming_publish: Received message with QoS 0 at topic:
                      distortos/0.7.0/ST,32F746GDISCOVERY/leds/0/state, payload length 1
[2020-04-18 11:50:15] mqttIncomingPublishCallback: topic =
                      "distortos/0.7.0/ST,32F746GDISCOVERY/leds/0/state", total length = 1
[2020-04-18 11:50:15] mqttIncomingDataCallback: length = 1, flags = 1
[2020-04-18 11:50:16] dns_tmr: dns_check_entries
```
