cf push counter-new --no-start
cf v3-set-health-check counter-new http --invocation-timeout 15
cf start counter-new
cf app counter-new | awk '/starting/{ system("sleep 1"); }'
cf app counter-new
cf map-route counter-new apps.pcf4u.com --hostname counter
cf unmap-route counter apps.pcf4u.com --hostname counter 
cf app counter
cf stop counter
cf delete counter
cf rename counter-new counter
