# simple-adder
- **Category:** misc
- **Points:** 100 (1 flag)
- **Description:** The final assignment of Dr. Adden McNumbar's class is simple: program a calculator. But for some reason, only one in 65536 students pass. Will you be one of them?
- **Flag 1:** 100 points
- **Access:** https://classroom.github.com/a/PjIaeYAx

## first look   
so its a github classroom assignment, we have to pass the automated github actions tests to get the flag without modifying the github actions workflow

## inspecting the code
```yml
      - name: test addition
        run: |
          # generate random numbers for test
          a=$RANDOM
          b=$RANDOM
          # save result
          sum=$(./add $a $b)
          if [[ $((a + b)) =~ $sum ]]; then
            echo "random=$a" >> $GITHUB_ENV
            echo "result=success" >> $GITHUB_ENV
            echo "sum=$sum" >> $GITHUB_ENV
          else
            echo "result=failure" >> $GITHUB_ENV
            echo "Expected $((a + b)) but got $sum"
            exit 1
          fi

      # I don't want unlucky students passing my class
      - name: test luck
        run: |
          # use new seed
          RANDOM=$random
          rand=$RANDOM
          if [ "$rand" = "22222" ]; then
            echo "lucky=true" >> $GITHUB_ENV
          else
            echo "lucky=false" >> $GITHUB_ENV
            echo "$rand is not the lucky number!"
            exit 1
          fi
```

this will run a file named add and check if the output is the same as the expected output, which is the sum of two random numbers
the second test checks if the random number is equal to 22222, if it is then we pass the test

## passing the first test
to pass the first test we need to create a file named add and make it executable
```bash
touch add
chmod +x add
```
then we need to write the code to add two numbers, this can be done in a few lines of code
```bash
#!/bin/bash
echo $(( $1 + $2 ))
```

this will take the two numbers passed as arguments and add them together, then echo the result, which is what the test is looking for

## passing the second test
this will be a little bit harder
the second test checks if the random number is equal to 22222, so we need to make sure that the random number is equal to 22222

the test uses the $RANDOM variable to generate a random number, so we can use this to our advantage since it is a pseudo-random number generator, meaning that it will generate the same number if we use the same seed
so we need to set the seed to 22222, this can be done by using the RANDOM variable again

we can do this by using a short bash script

I wrote a script that will test all the seeds until the first random number is equal to 22222

this is in the file "seedgen"

the number we got was 21615

now we have to somhow set the seed to 21615

## setting the seed
there is probably a few ways to do this, but the method we used was exploiting the fact that the github actions workflow is running in a bash shell, so we can set the seed by using the GITHUB_ENV variable

all we need to do is echo the seed to the GITHUB_ENV variable and it will be set as the seed for the next run of the workflow

```bash
#!/bin/bash
echo "RANDOM=21615" >> $GITHUB_ENV
```

testing this it doesn't work for somereason, so we tried a few different methods to trying to fix the issue and found that we also need to echo 'a' to the GITHUB_ENV variable since the test uses the generated random number as the seed for the next run of the workflow
so we need to set the seed to 21615 and then echo 'a' to the GITHUB_ENV variable
this will set the seed to 21615 and then echo 'a' to the GITHUB_ENV variable, which will be used as the seed for the next run of the workflow

```bash
#!/bin/bash
echo "RANDOM=21615" >> $GITHUB_ENV
echo -n "a" >> $GITHUB_ENV
```
pushing this code to the repo we can see that both tests pass, so we can go get checked off by our club Officer for the flag

** I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved **

