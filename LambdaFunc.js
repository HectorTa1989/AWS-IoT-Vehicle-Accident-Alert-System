console.log('Loading function');

var AWS = require('aws-sdk');
var https = require('https');
var sns = new AWS.SNS();
var dynamodb = new AWS.DynamoDB({apiVersion: '2012-08-10'});

exports.handler = function(event, context) {
    
    var appkey = 'X3hxTR';
    var privatekey = '5GH14gELwQXH';
    var channel = 'aws-iot';

    //Prepare latitude for generating Google Map hyperlink
    var latindex = event.state.reported.latitude.indexOf('.');
    var latitude = event.state.reported.latitude.slice(0,latindex-2)+'%'+event.state.reported.latitude.slice(latindex-2,event.state.reported.latitude.length);
    console.log("Latitude : "+latitude);
    //Prepare longitude for generating Google Map hyperlink
    var longindex = event.state.reported.longitude.indexOf('.');
    var longitude = event.state.reported.longitude.slice(0,latindex-2)+'%'+event.state.reported.longitude.slice(latindex-2,event.state.reported.longitude.length);
    console.log("Longtitude : "+longitude);
    //Prepare car's crashed time
    var timestamp = new Date(event.metadata.reported.car_number.timestamp*1000);
    console.log("Date : "+timestamp.toString());
    //Generate Alerting Message 
    var head = "Unfortunately, Your registered car has been crashed. We hope the following info will be helpful for you.\r\n\r\n";
    var car_number = "Car Number : " + event.state.reported.car_number +"\r\n";
    var location = "Location : http://maps.google.com/?q=" + latitude + ',' + longitude + "\r\n";
    var date = "Date : " +  timestamp.toString() + "\r\n";
    var emergency = "Emergency Services Number: Tourist Police(1155), Police(191) and Ambulance and Rescue(1554)";
    var alerting_message = head + car_number + location + date + emergency;
    //Send Alerting Message to Real Time Application
    var postBody = "AK=" + appkey + "&PK=" + privatekey + "&C=" + channel + "&M=" + alerting_message;
    var headers = {
      'Content-Type': 'application/x-www-form-urlencoded',
      'Content-Length': postBody.length
    };
    var options = {
      hostname: 'ortc-developers2-useast1-s0001.realtime.co',
      port: 443,
      path: '/send',
      method: 'POST',
      headers: headers
    };
    var req = https.request(options, function(res) {
        var body = '';
        res.setEncoding('utf8');
        res.on('data', function(chunk) {
            body += chunk;
        });
        res.on('end', function() {
            if(res.statusCode==201) {
                console.log('Message sent successfully to Realtime');
            } else {
                console.log('Error sending message to Realtime. Description: ', 
                body);
            }
            context.succeed(body);
        });
    });
    req.on('error', context.fail);
    req.write(postBody);
    req.end();
    //Send Alerting Message to SNS (Email)
    sns.publish({
        TopicArn: "arn:aws:sns:us-east-1:899237566099:carCrashSNS",
        Message: alerting_message
    }, function(err, data) {
        if(err) {
            console.error('error publishing to SNS');
            context.fail(err);
        } else {
            console.info('message published to SNS');
            context.done(null, data);
        }
    });
    //Insert Alerting information to DynamoDB
    var tableName = "CARCRASH_DB";
    dynamodb.putItem({
        "TableName": tableName,
        "Item" : {
            "CAR_NUMBER": {"S": event.state.reported.car_number },
            "DATE": {"S": timestamp.toString()},
            "LATITUDE": {"S": latitude },
            "LONGTITUDE": {"S": longitude }
        }
    }, function(err, data) {
        if (err) {
            context.done('error','putting item into dynamodb failed: '+err);
        }
        else {
            console.log('success: '+JSON.stringify(data, null, '  '));
        }
    });
};
