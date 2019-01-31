# cpp-warmup-then-fast
C++ code to simulate a Monolithic App that takes 10s to warm-up, and subsequently responds in 1s.
The delays are easily changed. The goal is to create a set of steps that demonstrate how PCF responds to these larger Apps.

## Assumptions to get started

- You know how to use PCF and the CF CLI at a basic level - i.e. you have successfully pushed apps in the past.
- You have access to a PCF (Org, Space) environment with enough quota to run this demo (20GB RAM).
- Your CF CLI will be able to access https://github.com/cloudfoundry-community/cppcms-buildpack

## Let's get started

- `$ cd /work`  I'm using my `/work` directory
- `$ git clone https://github.com/rm511130/cpp-warmup-then-fast`
- `$ cd cpp-warmup-then-fast`
- `$ cf push counter -b https://github.com/cloudfoundry-community/cppcms-buildpack --no-start` 

Did it all work? If you can't access the cppcms-buildpack, you'll need to ask your PCF Ops team to make it available to you as an offline buildpack: https://docs.run.pivotal.io/buildpacks/depend-pkg-offline.html

## Set Up your Display


