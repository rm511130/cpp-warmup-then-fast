# cpp-warmup-then-fast
C++ code to simulate a Monolithic App that takes 10s to warm-up, and subsequently responds in 1s.
The delays are easily changed. The goal is to create a set of steps that demonstrate how PCF responds to these larger Apps.

## Assumptions to get started

- You know how to use PCF and the CF CLI at a basic level - i.e. you have successfully pushed apps in the past.
- You have access to a PCF (Org, Space) environment with enough quota to run this demo (20GB RAM).
- Your CF CLI is able to access https://github.com/cloudfoundry-community/cppcms-buildpack
- You have Jmeter  `brew install jmeter --with-plugins`
- You have a page auto-refresh plug-in for your [browser](https://chrome.google.com/webstore/detail/page-refresh/hmooaemjmediafeacjplpbpenjnpcneg?hl=en)

## Let's get started

- `$ cd /work`  I'm using my `/work` directory
- `$ git clone https://github.com/rm511130/cpp-warmup-then-fast`
- `$ cd cpp-warmup-then-fast`
- `$ cf push counter -b https://github.com/cloudfoundry-community/cppcms-buildpack --no-start` 

Did it all work? If you can't access the cppcms-buildpack, you'll need to ask your PCF Ops team to make it available to you as an [offline buildpack.](https://docs.run.pivotal.io/buildpacks/depend-pkg-offline.html)

## Set Up your Display

In order to follow the demo-script, you should set-up your desktop per the example below. Yes, it looks crowded but as we advance through the demo you will see that being able to see every window is really useful.

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/Desktop.png)


## Demo Script

1. Explain to your audience what each terminal window is displaying (see example above)
2. [Edit](https://code.visualstudio.com/download) the `counter.cpp` app and walk through its contents to explain what it does:
   - The first time the code is executed, it waits for `delay_in_microsecs` before responding and it writes a `/home/vcap/app/first.txt` file to its container filesystem.
   - Subsequent executions of the C++ code wait for `delay_in_subsequent_loops` before responding back.
   - The response on the screen will look like one of the examples shown below:
   
   ![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/msg.png)
   
   - We can change font color to help exemplify blue/green deployments by altering the following line of code
   
   ` "<font color=\"black\">\n";`

   - There are additional features commented out. They may be used in the future to simulate memory leaks.
   
3. Set `delay_in_microsecs = 10000000` which equates to 10s and set `delay_in_subsequent_loops = 985000` which equates to 0.985s

4. Edit the `manifest.yml` file and walk through its contents. Make sure to set `instances: 1` and `health-check-type: port`. You should also comment out `# health-check-endpoint: /` per the example shown below:

```
applications:
- name: counter
# random-route: true
  disk_quota: 1G
  memory: 1G 
  instances: 1
  buildpack: https://github.com/cloudfoundry-community/cppcms-buildpack
  health-check-type: port
# health-check-endpoint: /
```

5. Switch off the auto-refresh function of the two browsers on the left of your display you covered in Step 1 (a.k.a. "the display" or "your display").

6. Using the terminal window in the middle of the display, issue the following command:

```
cf push
```

(we are assuming that you have already gone through the `cf api` and `cf login` steps in the recent past)

7. Using the terminal window immediately above, issue the command:

```
cf logs counter
```

8. Navigate to the Apps Manager window and make sure you can see the *Events, App Summary, Processes and Instances* under the *Overview* tab for the *Counter* app.

9. Once the *Counter* App is running, use one of the browsers on the left of your display to check how long it takes for the 1st response page to show up. Given that my *Apps Domain* is *apps.pcf4u.com* and my *hostname* is *counter*, I'm openning *http://counter.apps.pcf4u.com*

10. To confirm your findings, take a look at the terminal window dedicated to `cf logs counter` from Step 7:

```
   2019-01-30T22:14:04.89-0500 [APP/PROC/WEB/0] ERR 2019-01-31 03:14:04; cppcms_http, info: GET / (http_api.cpp:251)
   2019-01-30T22:14:14.90-0500 [RTR/2] OUT counter.apps.pcf4u.com - [2019-01-31T03:14:04.875+0000] "GET / HTTP/1.1" 200 0 197 "http://counter.apps.pcf4u.com/" "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36" "10.0.30.250:49028" "10.0.30.29:61030" x_forwarded_for:"172.16.0.14, 10.0.30.250" x_forwarded_proto:"http" vcap_request_id:"dd5a249e-48b9-4d7a-611f-5fe44cb746be" response_time:10.023769511 app_id:"55c2828c-86bb-477e-bc5c-7eecaa2b78ef" app_index:"0" x_b3_traceid:"1e56367e499a384a" x_b3_spanid:"1e56367e499a384a" x_b3_parentspanid:"-"
   2019-01-30T22:14:14.90-0500 [RTR/2] OUT
```

The log snippet shown above can be scrolled to show more details to the right-hand side. Look for *response_time* and you will see a number very close to 10s. You can ignore the first line in the log shown above, it will be shown in red and it corresponds to a minor warning message of the *cppcms* system.

11. Using the second browser, open the same page again *http://counter.apps.pcf4u.com* and check the *logs* to see that it now responded in about 1s. All consistent with the set-up we covered on Step 3. 

12. Set the auto-refresh of the two browsers to hit *http://counter.apps.pcf4u.com* every 2 seconds. The response times should all be very close to 1s.

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/autorefresh.png)
   
   


