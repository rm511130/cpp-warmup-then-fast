# cpp-warmup-then-fast
C++ code to simulate a Monolithic App that takes 10 seconds to warm-up, and subsequently responds in 1 second.
These time delays are easily changed in the code. Our goal is to create a set of steps that demonstrate how PCF responds to these larger Apps.

## Assumptions (Pre-requisites) to get started

- You know how to use PCF and the [CF CLI](https://docs.cloudfoundry.org/cf-cli/install-go-cli.html) at a basic level - i.e. you have successfully pushed apps in the past.
- You have the [Git CLI](https://git-scm.com/downloads) installed
- You have access to a PCF (Org, Space) environment with enough quota to run this demo (20GB RAM).
- You have [Java](https://www.java.com/en/) & [Jmeter](https://jmeter.apache.org/) i.e. `brew install jmeter --with-plugins`
- You have a page auto-refresh plug-in for your [browser](https://chrome.google.com/webstore/detail/page-refresh/hmooaemjmediafeacjplpbpenjnpcneg?hl=en)
- You have a recent [CF CLI](https://docs.cloudfoundry.org/cf-cli/install-go-cli.html) with the [Zero-Downtime-Push Plugin](https://github.com/contraband/autopilot)
- You have the CF Recycle plugin - `cf install-plugin -r CF-Community "cf-recycle-plugin"`

## An additional requirement

- The `cf buildpacks` command produces a list that includes the `cppcms-buildpack.zip` buildpack, and it's enabled.
- Otherwise, your Platform Administrator will have to:

  (1) Download the [cppcms-buildpack.zip](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/cppcms-buildpack.zip) file 
  
  (2) Execute `cf create-buildpack cppcms-buildpack ~/Downloads/cppcms-buildpack.zip 21 --enable`
  
  - (2a) `~/Downloads/cppcms-buildpack.zip` is the location of my downloaded [cppcms-buildpack.zip](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/cppcms-buildpack.zip) file 
  
  - (2b) `21` was the next available position where I decided to add the `cppcms-buildpack`
  
 - I'll proceed asuming that `cppcms-buildpack` is visible and enabled when you execute `cf builpacks`

   If that is not the case, but you have access to the [cppcms buildpack](https://github.com/cloudfoundry-community/cppcms-buildpack), you can use:
   
   `$ cf push counter -b https://github.com/cloudfoundry-community/cppcms-buildpack`


## Let's get started

- `$ cd /work`  I'm using my `/work` directory
- `$ git clone https://github.com/rm511130/cpp-warmup-then-fast`
- `$ cd cpp-warmup-then-fast`
- `$ cf push counter -b cppcms-buildpack --no-start` 

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
   
13. Let's `ssh` into the container and using `ps -ef` let's see what is running:

```
$ cf ssh counter
vcap@5ae32ebb-f7ea-4fa6-7647-38a9:~$ ps -ef
UID          PID    PPID  C STIME TTY          TIME CMD
root           1       0  0 03:02 ?        00:00:00 /tmp/garden-init
vcap          18       0  0 03:02 ?        00:00:00 make run
vcap          25       0  0 03:02 ?        00:00:00 /tmp/lifecycle/diego-sshd --allowedKeyExchanges= --address=0.0.0.0:2222 --allowUnauthenti
catedClients=false --inheritDaemonEnv=true --allowedCiphers= --allowedMACs= --logLevel=fatal --debugAddr=
vcap          64      18  0 03:02 ?        00:00:19 ./counter -c cppcms.js
root          72       0  0 03:02 ?        00:00:00 sh -c trap 'kill -9 0' TERM; /etc/cf-assets/envoy/envoy -c /etc/cf-assets/envoy_config/en
voy.yaml --service-cluster proxy-cluster --service-node proxy-node --drain-time-s 900 --log-level critical& pid=$!; wait $pid
root         113      72  0 03:02 ?        00:02:59 /etc/cf-assets/envoy/envoy -c /etc/cf-assets/envoy_config/envoy.yaml --service-cluster pr
oxy-cluster --service-node proxy-node --drain-time-s 900 --log-level critical
root         132       0  0 03:02 ?        00:00:02 /etc/cf-assets/healthcheck/healthcheck -port=8080 -timeout=1000ms -liveness-interval=30s
vcap         188      25  0 17:33 pts/0    00:00:00 /bin/bash
vcap         199     188  0 17:38 pts/0    00:00:00 ps -ef
```

The line corresponding to PID 132 shows: `healthcheck -port=8080 -timeout=1000ms -liveness-interval=30s`  i.e. every 30 seconds, it will try to establish a tcp connection on port 8080, if successful within 1s, it will consider the App to be healthy - otherwise, the `healthcheck` code will consider the App to be unhealthy and `kill -9` it to prompt PCF to register the crash event and respawn a new container.

_**Important Consequences to Note when using 'port' as your App's health-check-type**_

(a) The `kill -9` means that anyone using that App Instance will see a broken-pipe and experience a brief (under 1s) outage before PCF routers remove that App Instance from its valid routes table.

(b) The App crash event will be visible under `cf events counter` and `cf logs`.

(c) Even if we change `delay_in_subsequent_loops = 1100000`, i.e. 1.1s, making the App respond in slightly over 1s, the container running the App will probably respond to the TCP connection request on port 8080 in well under 1s, so both the initial delay of 10s (`delay_in_microsecs = 10000000`) and subsequent responses in 1.1s will not be considered a problem. 

(d) The App may be reach 100% CPU utilization, but `health-check-type = port` will probably not see it as a problem, as long as the TCP connection to port 8080 is established within 1s.

(e) If a container becomes unresponsive or an app memory leakage leads to an out-of-memory issue, the App/Container will crash and PCF will respawn the App instance in a new Container. **As soon as** the TCP connection to port 8080 is established on the new container, PCF will start routing traffic to it and, in our `Counter.cpp` example, the first request will take 10s (`delay_in_microsecs = 10000000`) before it sees any results - assuming that the App and its underlying Web Server are ready to respond. If the App is not ready to respond or is overwhelmed by a high number of requests it cannot handle, we may see an error.

14. Let's experience what the "Important Consequences" (described above) really mean:

We'll need to use JMeter to simulate some hundreds of users randomly using our `Counter` App. The command is:

`jmeter -n -t x-plan.jmx | awk '/Active/{ print $0; }'`

The `awk` part of the command is just to filter out summary information which will make the logs easier to read. 
The `x-plan.jmx` file is available in this repo and it looks like this:

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/Jmeter-plan.png)

So go ahead and start the JMeter test-suite in the bottom window of your display and observe what it reports back:

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/1_500_port.png)

Some interesting facts we need to acknowledge from the sample data shown above:

(a) Our test is using a single instance of our `Counter` App configured to have a warm-up period of 10s and to wait at least 985ms for subsequent requests.

(b) The single instance of `Counter` is overwhelmed by the 500 concurrent users and is responding, in average, after 20s of having received the request. However, the %CPU utilization is relatively low.

(c) No errors are being logged, so 100% of the user requests are being properly handled.

15. Leave the test running and lets `cf ssh` into the container where the `Counter` App is running:

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/top_n_topH.png)

We can deduce that the 21 threads running `Counter` explain the 20 responses/s we saw in the JMeter logs.

`Envoy` is doing all the hard work of handling the requests, and it's clearly using most of the CPU Time.

At 20 responses/s it takes 25s to allow all 500 concurrent users to get one response to one of their requests.

16. Let's switch on Autoscaling based on latency to get the response times down to less than 1500ms 

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/autoscaling.png)

17. Now that we have autoscaling setup and applied, let's see what happens

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/Effects_of_Autoscaling.png)

After a few minutes the overall system does stabilize at 8 App Instances with an average response time of 1.024s, a minimum response time of 1,001s and a max response time of 1,215s. If an App Instance is removed and later re-added by the autoscaling logic, we will see a max response time of 10s skewing the average up to 1.3s before it stabilizes again. 

We observed 83.1 responses/s to be the peak performance when 8 App Instances were actively handling the requests.

During my tests I did observe a handful of errors in the JMeter logs. The `plan1.csv` file holds the details. In every case, I was able to correlate the errors with network glitches reported by the router in my Lab Environment.

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/NetworkErrors.png)

18. Let's change the Counter App a little and then use [cf zero-downtime-push](https://github.com/contraband/autopilot)

(a) Switch-off autoscaling and scale manually to 8 App Instances (if not already there)

(b) Edit your `counter.cpp` file and change `delay_in_subsequent_loops = 985000` to `delay_in_subsequent_loops = 10000`

(c) And also change the `font color=\"black\"` to `font color=\"blue\"`

(d) Save the changes to `counter.cp`

(e) Keep the original `manifest.yml` content where you had specified `instances: 1` 

(f) Last step - using the middle terminal window on your display, proceed with the following command:

```cf zero-downtime-push counter -f manifest.yml```

You should see the following initial changes:

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/cf-zdt-push-1.png)

Using your App Manager UI, look for the new App that is being spun up in the same targeted Space:

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/cf-zdt-push-2.png)

As the new App takes over, you should see the following changes:

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/cf-zdt-push-3-blue.png)

So let's think about what just happened:

(a) The new blue-font Counter App takes 10s to warm-up, but it then responds in just 0.01s allowing a single instance to manage the same workload with a higher degree of %CPU utilization. By improving the App code we're able to make better use of the infrastructure.

(b) Zero-downtime-push worked without any errors even though we did register a max response time of 10s at the beginning when the blue-font Counter App started to handle the JMeter requests.

(c) The `cf logs counter` terminal window stoped working because it was linked to the GUID of the Counter App that was replaced and stopped. The consolidated logs continued to flow up and until the containers were terminated.

(d) Once the new blue-font Counter App was running, you were able to re-issue the `cf logs counter` and it started to show logs for the new App.

19. Scale the Blue-Font Counter App to 3 App Instances

```$ cf scale counter -i 3```

You should see an even distribution of the workload represented by smaller %CPU numbers in Apps Manager. During my tests I also saw a small improvement in the throughput: 100 responses/s.

20. Let's simulate App Failure and see what happens

(a) `cf ssh counter`

(b) `ps -ef` and look for the PID for `./counter -c cppcms.js`

(c) `kill -9 <PID of Counter App>` per the example shown below:

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/kill-9.png)

What happens immediately after you issue the `kill -9` command is quite interesting:

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/kill-9-results.png)

And seconds after that, here's a snapshot of what you should see:

![](https://github.com/rm511130/cpp-warmup-then-fast/blob/master/kill-9-results-part2.png)

If you execute this test a few times, you will see that sometimes the JMeter logs will register anything from a handfull of errors to 200+ errors under very similar conditions. It just depends on how many concurrent requests were in flight or scheduled to be handled by the App that was killed.

21. What happens when the JMeter App is less tolerant of delays

(a) Edit the `x-plan.jmx` file to change the following two lines from 60s to just 0.5s as shown below. Note that 0.5s is 6x the average response time we had been observing until now.

```
<stringProp name="HTTPSampler.connect_timeout">500</stringProp>
<stringProp name="HTTPSampler.response_timeout">500</stringProp>
```

(b) Re-start the JMeter tests against the 3 Blue-Font Counter App instances and observe the results:




























