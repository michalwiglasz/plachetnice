#include <stdlib.h>
#include <fitkitlib.h>
#include "servo.h"
#include "ctrl_reg.h"

// stezen
servo_t s_mast = {
  .name = "Stezen",
  .addr = 0x00A0,
  .period = SERVO_HS422_PERIOD,
  .left = 10000,  // 500 us
  .right = 46000,  // 2300 us
  .center = 28000,  // 1400 us
  .left_angle = -90,
  .right_angle = 90,
  .center_angle = 0,
  .ctrlreg_bit = 15,
};

servo_t s_wheels = {
  .name = "Podvozek",
  .addr = 0x00B0,
  .period = SERVO_HS422_PERIOD,
  .left = 20800,  // 1040 us
  .right = 34800,  // 1740 us
  .center = 27800,  // 1390 us
  .left_angle = -45,
  .right_angle = 45,
  .center_angle = 0,
  .ctrlreg_bit = 14,
};

void print_info();

/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
void print_user_help(void)
{
  term_send_str("Plachetnice:"); //Vypis retezce na terminal
  term_send_crlf();        //Prechod na novy radek
}

/*******************************************************************************
 * Dekodovani uzivatelskych prikazu a jejich vykonavani
*******************************************************************************/
unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
  if (strcmp2(cmd_ucase, "BS")) {
    int bit = cmd[2] - '0';
    if (bit >= 0 && bit <= 9) {
      ctrlreg_set_bit(bit);

    } else {
      term_send_str("Neplatny bit (0 .. 9).\n");
    }

  } else if (strcmp2(cmd_ucase, "BC")) {
    int bit = cmd[2] - '0';
    if (bit >= 0 && bit <= 9) {
      ctrlreg_clear_bit(bit);

    } else {
      term_send_str("Neplatny bit (0 .. 9).\n");
    }

  } else if (strcmp2(cmd_ucase, "BG")) {
    int bit = cmd[2] - '0';
    if (bit >= 0 && bit <= 9) {
      int val = ctrlreg_get_bit(bit);
      term_send_str(val? "on" : "off");

    } else {
      term_send_str("Neplatny bit (0 .. 9).\n");
    }

  } else if (strcmp2(cmd_ucase, "MW")) {
    uint16_t usec = strtoul(cmd + 2, NULL, 10);
    if (usec) {
      servo_set_width(&s_mast, usec);

    } else {
      term_send_str("Zadejte platnou sirku stridy.\n");
    }

  } else if (strcmp2(cmd_ucase, "ML")) {
      servo_go_left(&s_mast);

  } else if (strcmp2(cmd_ucase, "MC")) {
      servo_go_center(&s_mast);

  } else if (strcmp2(cmd_ucase, "MR")) {
      servo_go_right(&s_mast);

  } else if (strcmp2(cmd_ucase, "MN")) {
      ctrlreg_set_bit(s_mast.ctrlreg_bit);

  } else if (strcmp2(cmd_ucase, "MF")) {
      ctrlreg_clear_bit(s_mast.ctrlreg_bit);

  } else if (strcmp1(cmd_ucase, "M")) {
    long angle = strtol(cmd + 1, NULL, 10);
    servo_set_angle(&s_mast, angle);



  } else if (strcmp2(cmd_ucase, "WW")) {
    uint16_t usec = strtoul(cmd + 2, NULL, 10);
    if (usec) {
      servo_set_width(&s_wheels, usec);

    } else {
      term_send_str("Zadejte platnou sirku stridy.\n");
    }

  } else if (strcmp2(cmd_ucase, "WL")) {
      servo_go_left(&s_wheels);

  } else if (strcmp2(cmd_ucase, "WC")) {
      servo_go_center(&s_wheels);

  } else if (strcmp2(cmd_ucase, "WR")) {
      servo_go_right(&s_wheels);

  } else if (strcmp2(cmd_ucase, "WN")) {
      ctrlreg_set_bit(s_wheels.ctrlreg_bit);

  } else if (strcmp2(cmd_ucase, "WF")) {
      ctrlreg_clear_bit(s_wheels.ctrlreg_bit);

  } else if (strcmp1(cmd_ucase, "W")) {
    long angle = strtol(cmd + 1, NULL, 10);
    servo_set_angle(&s_wheels, angle);

  } else if (strcmp1(cmd_ucase, "I")) {
    print_info();

  } else {
    return (CMD_UNKNOWN);
  }

  return USER_COMMAND;
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
void fpga_initialized()
{
}


void print_servo_info(servo_t *servo) {
  term_send_str("Servo \"");
  term_send_str(servo->name);

  if (ctrlreg_get_bit(servo->ctrlreg_bit))
    term_send_str("\" ZAPNUTO");
  else
    term_send_str("\" VYPNUTO");

  term_send_str("\nAdresa: ");
  term_send_num(servo->addr);

  term_send_str("\nPerioda: ");
  term_send_num(servo->period);

  term_send_str("\nLevy okraj: ");
  term_send_num(servo->left);
  term_send_str(" (");
  term_send_num(servo->left_angle);
  term_send_str("°)");

  term_send_str("\nPravy okraj: ");
  term_send_num(servo->right);
  term_send_str(" (");
  term_send_num(servo->right_angle);
  term_send_str("°)");

  term_send_str("\nStred: ");
  term_send_num(servo->center);
  term_send_str(" (");
  term_send_num(servo->center_angle);
  term_send_str("°)");

  uint16_t w = servo_get_width(servo);
  term_send_str("\nAktualni poloha: ");
  term_send_num(w);
  term_send_str(" (");
  term_send_num(servo_width_to_angle(servo, w));
  term_send_str("°)");
  term_send_crlf();
}


void print_info()
{
  print_servo_info(&s_mast);
  term_send_crlf();
  print_servo_info(&s_wheels);
}

/*******************************************************************************
 * Hlavni funkce
*******************************************************************************/
int main(void)
{
  initialize_hardware();
  servo_init(&s_mast);
  servo_init(&s_wheels);

  ctrlreg_set_bit(s_mast.ctrlreg_bit);
  ctrlreg_set_bit(s_wheels.ctrlreg_bit);

  delay_ms(100);
  print_info();

  while (1) {
    delay_ms(10);
    terminal_idle();  // obsluha terminalu
  }
}

