#pragma once
#include <memory>
#include <string>
#include "../models/User.h"
#include "../interface/UserService.h"
#include "../interface/WalletService.h"
#include "walletScreen.h"

void showAdminScreen(std::shared_ptr<User> admin, UserService& userService, WalletService& walletService); 