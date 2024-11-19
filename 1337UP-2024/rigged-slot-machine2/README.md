# Rigged Slot Machine 2
> The casino fixed their slot machine algorithm - good luck hitting that jackpot now! 🤭
> nc riggedslot2.ctf.intigriti.io 1337 

## Solution
connecting to the server prompts us to enter in our name, and then after that we are able to "bet"

looking at the binary in ghidra we can see that the binary is a simple slot machine game, but there is a few things to note

```c
void main(void) {
  time_t tVar1;
  int local_2c;
  undefined local_28 [20];
  uint local_14;
  int local_10;
  __gid_t local_c;
  
  setvbuf(stdout,(char *)0x0,2,0);
  local_c = getegid();
  setresgid(local_c,local_c,local_c);
  tVar1 = time((time_t *)0x0);
  srand((uint)tVar1);
  setup_alarm(5);
  local_14 = 100;
  puts("Welcome to the Rigged Slot Machine!");
  puts("You start with $100. Can you beat the odds?");
  enter_name(local_28);
  do {
    while( true ) {
      while( true ) {
        local_2c = 0;
        printf("\nEnter your bet amount (up to $%d per spin): ",100);
        local_10 = __isoc99_scanf(&DAT_0010224e,&local_2c);
        if (local_10 == 1) break;
        puts("Invalid input! Please enter a numeric value.");
        clear_input();
      }
      if ((local_2c < 1) || (100 < local_2c)) break;
      if ((int)local_14 < local_2c) {
        printf("You cannot bet more than your Current Balance: $%d\n",(ulong)local_14);
      }
      else {
        play(local_2c,&local_14);
        if (local_14 == 0x14684c) {
          payout(&local_14);
        }
      }
    }
    printf("Invalid bet amount! Please bet an amount between $1 and $%d.\n",100);
  } while( true );
}
```

we can see that there is some input sanitzation when betting, but when you enter in your name there is none.

we can use this to our advantage to overflow the buffer and overwrite the local_14 variable which holds our balance.

in another function we can see how we get the flag:

```c
void payout(int *param_1) {
  char local_58 [72];
  FILE *local_10;
  
  if (*param_1 != 0x14684c) {
    puts("You can\'t withdraw money until you win the jackpot!");
                    // WARNING: Subroutine does not return
    exit(-1);
  }
  local_10 = fopen("flag.txt","r");
  if (local_10 == (FILE *)0x0) {
    puts(
        "Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server."
        );
                    // WARNING: Subroutine does not return
    exit(0);
  }
  fgets(local_58,0x40,local_10);
  printf("Congratulations! You\'ve won the jackpot! Here is your flag: %s\n",local_58);
  fclose(local_10);
  return;
}
```

we can see that we need to have our balance be EXACTLY 0x14684c to get the flag, which is $1337419.

translating this to ascii we get: hL

the array name is 20 bytes long, so we need a username of 20bytes then 2 bytes of padding, then the 4 bytes of the balance.

so the password looks something like this:
    
    ```shell
    aaaaaaaaaaaaaaaaaaaahL
    ```

trying it out, it does not give us the correct balance, it gave us, something way off, reversing the bytes we get the correct balance.

    ```shell
    aaaaaaaaaaaaaaaaaaaaLh
    ```

this gets us closer to the value, but not quite: 1337419

converting 1337419 to hex we get 0x14664b, wich is exaclty 1 less than the value we need. which is also our bet value

so the actual hex value we need is 0x14664d

converting this to ascii we get and revesring it, then appending it to our garbage value gets us the exact value we need: 1337419

and boom, there is our flag:

    ```shell
    INTIGRITI{1_w15h_17_w45_7h15_345y_1n_v3645}
    ```