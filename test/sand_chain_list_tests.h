#pragma once

#include <gb/gb.h>

BANKREF_EXTERN(test__sand_chain_list)

/******* INSTANCE *******/
void test__sand_chain_list__contains() BANKED;
void test__sand_chain_list__contains_x() BANKED;
void test__sand_chain_list__has_any() BANKED;
void test__sand_chain_list__push_front() BANKED;
void test__sand_chain_list__push_front_reference() BANKED;
void test__sand_chain_list__pop_front() BANKED;
void test__sand_chain_list__get_last() BANKED;

/******* CLASS *******/
void test__sand_chain_list__combine() BANKED;