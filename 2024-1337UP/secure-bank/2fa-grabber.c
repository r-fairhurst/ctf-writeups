
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare the obscure_key function
uint32_t obscure_key(uint32_t param_1){
  return ((param_1 ^ 0xa5a5a5a5) << 3 | (param_1 ^ 0xa5a5a5a5) >> 0x1d) * 0x1337 ^ 0x5a5a5a5a;
}

uint32_t generate_2fa_code(int param_1)
{
  uint32_t local_14;
  uint32_t local_10;
  uint32_t local_c;
 
  local_10 = param_1 * 0xbeef;
  local_c = local_10;
  for (local_14 = 0; local_14 < 10; local_14 = local_14 + 1) {
  local_c = obscure_key(local_c);
  local_10 = ((local_10 ^ local_c) << 5 | (local_10 ^ local_c) >> 0x1b) +
            (local_c << ((char)local_14 + (char)(local_14 / 7) * -7 & 0x1fU) ^
            local_c >> ((char)local_14 + (char)(local_14 / 5) * -5 & 0x1fU));
  }
  return local_10 & 0xffffff;
}

int main(){
  int param_1 = 1337;
  printf("2FA code: %d\n", generate_2fa_code(param_1));
  return 0;
}