# Virtual Queue Web Server


### Backend Program Flow:
1) Store Owner -> Web Server
```
Initialize store capacity 
```

2) Customer -> Web Server 
```
Customer inserts name, phone number, number of people
```

3) Web Server -> Customer
```
Web Server generates QR code from string[hash(name + phone number) + numOfPeople:xx]
```

4) Customer -> MCU -> Web Server
```
Customer scans QR code. MCU parses the QR code and send GET request to Web Server. Web Server decides if a customer can come into the store or not 
```

5) MCU -> Web Server
```
IR sensors detect people going out
```

6) Web Server -> Customer (Only if there is customer on the queue)
```
Notify new customer
```

### Backend TODO:
- [ ] Need an API query to set up the store capacity
- [ ] Need an API (can only be accessed by the worker to kick people out of queue)
- [ ] Need an API to generate QR code
- [ ] Need a function to validate if customer is inserting a valid Name and number (regex)??
- [ ] Need a parser for the STM32 to decode the string
- [ ] Check if hash is within `int` range