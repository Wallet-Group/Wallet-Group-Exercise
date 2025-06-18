#pragma once
#include <memory>
#include <string>
#include "../models/User.h"
#include "../interface/UserService.h"
#include "../interface/WalletService.h"
#include "walletScreen.h"

void showUserScreen(std::shared_ptr<User> user, UserService& userService, WalletService& walletService); 