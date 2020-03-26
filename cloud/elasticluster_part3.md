Working with the grid
======================

## Step 1. Log into a machine where I have installed ARC client for you.

You each have a username and home folder there.

```
ssh -i ~/.ssh/inf9380-ssh studentNN@158.39.48.136
```

## Step 2. Set up your authentication for ARC. 

You need to authenticate yourself when you submit a job to the grid.

We use x509 certificates for that. You have a test-certificate already created for you that you must activate.
```
source arc-testca-usercert/setenv.sh
arcproxy
```
Your credentials are now saved to environment variables that are accesses by the ARC client.



## Step 3. Inspect the test-job files. 

Go into the testing directory.

Inspect runhello.sh
```
cat ~/testing/runhello.sh

```

Inspect hello_inputfile.xrls

```
cat hello_inputfile.xrls
```


Inspect submit-cmd.sh
```
cat submit-cmd.sh
```

## 4. Submit the job
Either copy-paste the command in submit-cmd.sh or
```
source submit-cmd.sh
```

## 5. Inspect the job
If all went well the ARC client returns the jobid. Have a look at the job, using the full url of the job. 

```
arcstat  <jobid>
```

Example:

```
arcstat  https://158.39.48.12:443/arex/1LiLDmAaobwne07Iwmq4uREmCMOcFnABFKDmYRFKDmFBFKDmnqo9Xo
```

Check all jobs that you have submitted
```
arcstat -a
```


If the job is finished download the results

```
arcget <jobid>
```

Example:

```
arcget https://158.39.48.12:443/arex/1LiLDmAaobwne07Iwmq4uREmCMOcFnABFKDmYRFKDmFBFKDmnqo9Xo
```

