# Automatic Vehicle Accident Alert System using AWS IoT

In vehicle accident, An alerting message which contains time and location automatically is sent to emergency agencies for relief the victim.

According to the Top 10 Causes of Death Report by CDC, Road Injuries hit in several country including my country, Thailand that 4 percent of death is a result of the vehicle accident. Frequently, the serious accidents occur at nighttime; in that case, it might be not caught sight of other people then the victim who is unable to call some emergency services by himself don’t get help at the right time. With Automatic Vehicle Accident Alert System, an alerting message which contains accident’s time and location sent automatically by the particular device embedded in a vehicle to emergency and relief agencies. So, they can rush to the accident and help the victim in time.

          I use the amazing AWS IoT along with SNS, DynamoDB and Lambda for this project. AWS IoT enables communication between Internet-connected vehicle-embedded devices and the AWS cloud over MQTT. About alert services, AWS SNS is used for alerting an involved person via E-mail and Real-time Framework handle a real-time messaging task. DynomoDB is used for store accident information such as car number, time, latitude and longitude. Indispensably, AWS Lambda take a role of a manager who invokes the other services.

         Additionally, I would rather make a comparison about system developing using AWS IoT and vice versa. 3 years ago when I was a college student, I developed a simply home automation system using Arduino. It took a long time because I had to set up the Apache web server for communicating with devices via HTTP. Contrariwise, this project is more thoroughly and took a little time for developing. Thus, AWS IoT provide a great environment for “Internet of Things”.



Part 1: Setup Raspberry Pi 2
Step 1: Follow all steps in the following link to setup, connect and flash the image for Raspberry Pi 2.

https://www.raspberrypi.org/help/noobs-setup/

(Cr. Raspberrypi.org)

Step 2: Download and install WiringPi

http://wiringpi.com/download-and-install/

(Cr. WiringPi)

Step 3: Set up AWS IoT C SDK

http://docs.aws.amazon.com/iot/latest/developerguide/iot-device-sdk-c.html

(Cr. Amazon)



Part 2: Create AWS Environment
Step 1: Setup the AWS IoT

Step 1.1: Login to the Amazon AWS Console. Then, select AWS IoT service.
![image](https://user-images.githubusercontent.com/31132150/145937152-74a03dca-3549-4ab2-a612-1f208c4723de.png)

Step 1.2: Create a thing. Type a thing name and its attributes.
![image](https://user-images.githubusercontent.com/31132150/145937207-971dd031-4977-492d-8682-5199e8e93692.png)

Step 1.3: Create a certificate. Then, download all the key & certificate files.
![image](https://user-images.githubusercontent.com/31132150/145937217-0aa76e94-3fd2-424e-9a52-eceb09eeb8a6.png)

Step 1.4: Create a policy. Type a policy name and add a policy statement.
![image](https://user-images.githubusercontent.com/31132150/145937226-261c65fb-6a5c-4df1-829b-f64fb1df71cb.png)

Step 1.5: Activate a certificate. Then, attach a policy and a thing to the certificate.
![image](https://user-images.githubusercontent.com/31132150/145937234-f7df3bc8-006e-4230-9325-b9c142335d7d.png)

Step 1.6: Create a rule. (After Complete Step 5)
![image](https://user-images.githubusercontent.com/31132150/145937240-d74f1557-ce24-44cd-830a-865d399646ac.png)


Step 2: Setup the AWS DynamoDB

Step 2.1: Login to the Amazon AWS Console. Then, select AWS DynamoDB service.
![image](https://user-images.githubusercontent.com/31132150/145937250-662d6ed0-47e3-4f68-b97d-ce0a9e84f734.png)

Step 2.2: Select Create table. Type Table name and Primary key.
![image](https://user-images.githubusercontent.com/31132150/145937261-c195cc48-4bf9-422a-adc0-98f18170e8cb.png)

Step 2.3: Select Manage Stream. Choose New and old images Type.
![image](https://user-images.githubusercontent.com/31132150/145937273-622c2929-baad-4a39-beb5-43ad2d18425d.png)

Step 3: Setup the AWS SNS

Step 3.1: Login to the Amazon AWS Console. Then, select AWS SNS service.
![image](https://user-images.githubusercontent.com/31132150/145937290-9faa887d-2f76-43ff-beea-0598fb844a23.png)

Step 3.2: Select Create Topic. Type Topic name and Display name.
![image](https://user-images.githubusercontent.com/31132150/145937301-81cb62ae-94c4-4991-9736-2f4c48d26bfe.png)

Step 3.3: Create a topic’s subscription. Then, confirm it by follow the link in your E-mail.
![image](https://user-images.githubusercontent.com/31132150/145937322-f5cc097a-7985-47f5-ae86-79326a871ebb.png)

Step 4: Setup IAM Roles

Step 4.1: Login to the Amazon AWS Console. Then, select AWS IAM service.
![image](https://user-images.githubusercontent.com/31132150/145937377-c7b6f96c-e6f2-4dd5-a81b-cef226aea6da.png)

Step 4.2: Create New Users. Enter a user name. Download Credentials.
![image](https://user-images.githubusercontent.com/31132150/145937402-a6e2c421-7072-47f2-bad5-f4e361abe77c.png)

Step 4.3: Select Permission. Attach Policy.
![image](https://user-images.githubusercontent.com/31132150/145937428-df723818-f815-43c3-94fa-6caeed129645.png)

Step 5: Create the Lambda Function

Step 5.1: Login to the Amazon AWS Console. Then, select AWS Lambda service.

![image](https://user-images.githubusercontent.com/31132150/145937446-bb47c766-9b9d-45fc-8919-257a7c2b9ef3.png)

Step 5.2: Select create a Lambda function. Then, configure function. (Source file’s in the code section)
![image](https://user-images.githubusercontent.com/31132150/145937457-b47394c6-e182-489a-b345-bfdc7016e819.png)

Step 5.3: Add Event source.
![image](https://user-images.githubusercontent.com/31132150/145937480-59efd055-457c-4b39-aa24-488e640e33ad.png)

Step 5.4: Configure following Input test event. Test Lambda function.
![image](https://user-images.githubusercontent.com/31132150/145937492-96c942c1-4ab5-4732-86fa-417c34d168f4.png)

Step 5.5: It is work fine.
![image](https://user-images.githubusercontent.com/31132150/145937516-586323a8-998c-42c6-9415-f51f28d7f1a1.png)

![image](https://user-images.githubusercontent.com/31132150/145937523-cac2904f-3cba-4435-9674-324c18c43073.png)

Now, AWS environment is completely ready for the system. Next, we will set up Real-time Framework.

Part 3: Setup Real-time Framework
Step 1: Register the Real-time Framework account from URL: https://realtime.co/ by using Facebook or Google Plus account.
![image](https://user-images.githubusercontent.com/31132150/145937618-018c198f-4314-4a6a-a353-fa1dbbc2ed17.png)

Step 2: Add Subscription. Select Realtime Cloud Messaging. Type Subscription name.
![image](https://user-images.githubusercontent.com/31132150/145937635-5876da26-4164-4764-9484-48e881fc31fd.png)

Step 3: Note Application Key and Private Key (Replace it instead of my keys in Lambda Code)
![image](https://user-images.githubusercontent.com/31132150/145937654-21f0e4fc-5055-4565-b0e2-e0910651b801.png)

Step 4: Open Real-time console. Connect and Subscribe to “aws-iot” channel. Then test again from Lambda function.
![image](https://user-images.githubusercontent.com/31132150/145937675-0e79df8f-94c1-47a4-a363-7724c2acae82.png)

Part 4: Build and run the Thing’s Agent
Step 1: Download source file and make it on Raspberry Pi 2 (Source file’s in the code section)

Step 2: View the output on the Monitor
![image](https://user-images.githubusercontent.com/31132150/145937693-b7fd095e-12b9-4932-9473-f4ef56ebd43a.png)

Step 3: Verify that car number change from aa000 to wc4444
![image](https://user-images.githubusercontent.com/31132150/145937706-8c429f83-804c-446e-8d70-5d70185d7399.png)

  Overall back-end system is now completely created. Then, we will go into assembling the prototype car.
  
  
  Part 5: Assemble the prototype car

Step 1: Collecting the right components
![image](https://user-images.githubusercontent.com/31132150/145937803-486c88d4-445a-4e18-9e16-60c358de0751.png)

Step 2: Connect all of the components
![image](https://user-images.githubusercontent.com/31132150/145937809-3831c4e8-142a-4e75-a873-b3922b452477.png)

Part 6: Test System
After I rush the prototype car with the wall, I have received following Alerting E-mail.
![image](https://user-images.githubusercontent.com/31132150/145937834-e6ccd808-e772-4549-9c8f-b89b270aee9d.png)

Part 7: The Improvement for real vehicle
          In order to assembly with real vehicle, this system need connecting with some crash sensors like front impact sensors and receiving a crashing signal from the sensors before invoke overall system function.
          ![image](https://user-images.githubusercontent.com/31132150/145937847-011d8501-fb1e-4a54-9125-5d52ddd4f733.png)


# Schematics
![image](https://user-images.githubusercontent.com/31132150/145937887-b9b7a43d-9114-4ab8-a3e2-0e84d49d9ed5.png)

# Hardware Schematic
![image](https://user-images.githubusercontent.com/31132150/145937905-a267d735-15e0-4611-a9cd-a3debaaa82cc.png)

