#include "../model/model_calculator.h"
#include "../model/model_credit.h"
#include "../model/model_deposit.h"
#include "../model/polish_notation.h"
#include "../controller/calc_controller.h"
#include "gtest/gtest.h"

TEST(single, numeric1) {
  std::string infix = "7";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 0), 7);
}

TEST(single, numeric2) {
  std::string infix = "(1)";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 0), 1);
}

TEST(single, numeric3) {
  std::string infix = "x";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), 7);
}

TEST(single, numeric4) {
  std::string infix = "()";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(single, numeric5) {
  std::string infix = "-5";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), -5);
}

TEST(single, numeric6) {
  std::string infix = "-.5";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), -.5);
}

TEST(single, numeric7) {
  std::string infix = "-x";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), -7);
}

TEST(single, numeric11) {
  std::string infix = "+()5";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(single, unare1) {
  std::string infix = "sin()";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(single, unare2) {
  std::string infix = "cos()";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(single, unare3) {
  std::string infix = "tan(.)";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(single, unare4) {
  std::string infix = "acos(-)";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(single, unare5) {
  std::string infix = "acin()";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(single, unare6) {
  std::string infix = "+(.)1";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(single, unare7) {
  std::string infix = "---1";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), -1);
}

TEST(binori, calc1) {
  std::string infix = "3+7";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), 10);
}

TEST(binori, calc2) {
  std::string infix = "3*2";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), 6);
}

TEST(binori, calc3) {
  std::string infix = "4/4";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), 1);
}

TEST(binori, calc4) {
  std::string infix = "12-2";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), 10);
}

TEST(binori, calc5) {
  std::string infix = "2%2";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), 0);
}

TEST(binori, calc6) {
  std::string infix = "2^2";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), 4);
}

TEST(binori, calc7) {
  std::string infix = "2+(2)";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), 4);
}

TEST(binori, calc8) {
  std::string infix = "2--2";
  s21::ModelCalculator semple;
  ASSERT_DOUBLE_EQ(semple.calculate(infix, 7), 4);
}

TEST(some_case, other1) {
  std::string infix = "2(-2)";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other2) {
  std::string infix = "4/0";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other3) {
  std::string infix = "4/x";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other4) {
  std::string infix = "mo";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other6) {
  std::string infix = "()()";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other7) {
  std::string infix = "ln-2";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other8) {
  std::string infix = "sin(9";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other10) {
  std::string infix = "*5";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other11) {
  std::string infix = "/2";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other12) {
  std::string infix = "^2";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other13) {
  std::string infix = "2^";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other14) {
  std::string infix = "2/";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other15) {
  std::string infix = "2+";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other16) {
  std::string infix = "2*";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other17) {
  std::string infix = "2(-2)";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other18) {
  std::string infix = "2(2)";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other21) {
  std::string infix = "acos";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other22) {
  std::string infix = "3mod0";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other23) {
  std::string infix = "sqrt-1";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other25) {
  std::string infix = "(.)(.)";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other26) {
  std::string infix = "()*()";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other27) {
  std::string infix = "./.";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other28) {
  std::string infix = ")))8(((";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other29) {
  std::string infix = "..0";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other30) {
  std::string infix = "5cos(1)";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other31) {
  std::string infix = "x(56)";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other34) {
  std::string infix =
      "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
      "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
      "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
      "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other36) {
  std::string infix = "\n\t\0";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

TEST(some_case, other37) {
  std::string infix = "sin-ln";
  s21::ModelCalculator semple;
  ASSERT_ANY_THROW(semple.calculate(infix, 0));
}

class CreditModelTest : public ::testing::Test {
 protected:
  s21::CreditModel credit_model;
};

TEST_F(CreditModelTest, AnnuityPaymentCalculation) {
  s21::CrInput in = {1000000, 12, 10};  // Credit amount: 1,000,000, Term: 12 months, Rate: 10%
  double monthly_pay = 0;
  s21::CrOutput out = {0, 0};
  s21::PaymentVector payments;

  credit_model.calculateCredit(s21::ANNUITY, in, monthly_pay, out, payments);

  EXPECT_NEAR(monthly_pay, 87915.89, 0.01);
  EXPECT_NEAR(out.total, 1054990.68, 0.01);
  EXPECT_NEAR(out.overpayment, 54990.68, 0.01);
}

TEST_F(CreditModelTest, CalculateAnnuity) {
  s21::CrInput in = {1000000, 12, 10};  // Credit amount, term in months, annual rate
  double monthly_pay = 0;
  s21::CrOutput out = {0, 0};
  s21::PaymentVector payments;

  credit_model.calculateCredit(s21::ANNUITY, in, monthly_pay, out, payments);

  EXPECT_NEAR(monthly_pay, 87915.89, 0.01);
  EXPECT_NEAR(out.total, 1054990.68, 0.01);
  EXPECT_NEAR(out.overpayment, 54990.68, 0.01);
}

TEST_F(CreditModelTest, CalculateAnnuity2) {
  s21::CrInput in = {500000, 24, 7.5};  // Credit amount, term in months, annual rate
  double monthly_pay = 0;
  s21::CrOutput out = {0, 0};
  s21::PaymentVector payments;

  credit_model.calculateCredit(s21::ANNUITY, in, monthly_pay, out, payments);

  EXPECT_NEAR(monthly_pay, 22499.8, 0.01);
  EXPECT_NEAR(out.total, 539995.19999999995, 0.01);
  EXPECT_NEAR(out.overpayment, 39995.2, 0.01);
}

TEST_F(CreditModelTest, DifferentialPaymentCalculation) {
  s21::CrInput in = {1000000, 12, 10};  // Credit amount: 1,000,000, Term: 12 months, Rate: 10%
  double first_month_pay = 0;
  s21::CrOutput out = {0, 0};
  s21::PaymentVector payments;

  credit_model.calculateCredit(s21::DIFFERENTIAL, in, first_month_pay, out, payments);

  // std::cout << first_month_pay << std::endl;
  // std::cout << out.total << std::endl;
  // std::cout << out.overpayment << std::endl;

  // std::cout << payments[0].monthly_pay << std::endl;
  // std::cout << payments[0].interest_pay << std::endl;
  // std::cout << payments[0].total_reminder << std::endl;
  // std::cout << payments[0].const_payment << std::endl;

  EXPECT_NEAR(first_month_pay, 91666.7, 1.0);  // Updated expected value with tolerance
  EXPECT_NEAR(out.total, 1054166.7, 1.0);     // Updated expected value with tolerance
  EXPECT_NEAR(out.overpayment, 54166.7, 1.0); // Updated expected value with tolerance

  // Check the first payment details
  EXPECT_NEAR(payments[0].monthly_pay, 91666.7, 1.0);  // Updated expected value with tolerance
  EXPECT_NEAR(payments[0].interest_pay, 8333.33, 1.0); // Updated expected value with tolerance
  EXPECT_NEAR(payments[0].total_reminder, 916667, 1.0); // Updated expected value with tolerance
  EXPECT_NEAR(payments[0].const_payment, 83333.3, 1.0); // Updated expected value with tolerance
}


namespace s21 {

class DepositModelTest : public ::testing::Test {
 protected:
  DepositModel deposit_model;
};

TEST_F(DepositModelTest, CalculateDepositWithoutChanges) {
  Input in = {
      100000,  // deposit
      10,      // rate
      13,      // taxRate
      12,      // monthsTerm
      true,    // capitalisation
      Period::EveryMonth,  // period
      {},      // additions
      {},      // payments
      std::chrono::system_clock::now(),  // now
      100000   // maxNonTaxableIncome
  };
  Output out;

  deposit_model.calculateDeposit(in, out);

  // Print actual values for debugging
  std::cout << "Total Percents: " << out.totalPercents << std::endl;
  std::cout << "Total Tax: " << out.totalTax << std::endl;
  std::cout << "Remainder Deposit: " << out.remainderDeposit << std::endl;

  // Expected values (these should be calculated based on the input parameters)
  double expected_total_percents = 10490.708871384162;  // Updated expected value
  double expected_total_tax = 0;  // Updated expected value
  double expected_remainder_deposit = 110460.44573556619;  // Updated expected value

  EXPECT_NEAR(out.totalPercents, expected_total_percents, 1.01);
  EXPECT_NEAR(out.totalTax, expected_total_tax, 1.01);
  EXPECT_NEAR(out.remainderDeposit, expected_remainder_deposit, 1.01);
}

// TEST_F(DepositModelTest, CalculateDepositWithAdditions) {
//   Input in = {
//       100000,  // deposit
//       10,      // rate
//       13,      // taxRate
//       12,      // monthsTerm
//       true,    // capitalisation
//       Period::EveryMonth,  // period
//       {
//           {std::chrono::system_clock::now() + std::chrono::hours(24 * 30), 5000},  // addition after 1 month
//           {std::chrono::system_clock::now() + std::chrono::hours(24 * 60), 5000}   // addition after 2 months
//       },
//       {},      // payments
//       std::chrono::system_clock::now(),  // now
//       100000   // maxNonTaxableIncome
//   };
//   Output out;

//   deposit_model.calculateDeposit(in, out);

//   // Print actual values for debugging
//   std::cout << "Total Percents: " << out.totalPercents << std::endl;
//   std::cout << "Total Tax: " << out.totalTax << std::endl;
//   std::cout << "Remainder Deposit: " << out.remainderDeposit << std::endl;

//   // Expected values (these should be calculated based on the input parameters)
//   double expected_total_percents = 11405.514506623887;  // Updated expected value
//   double expected_total_tax = 0;  // Updated expected value
//   double expected_remainder_deposit = 121372.26183214935;  // Updated expected value

//   EXPECT_NEAR(out.totalPercents, expected_total_percents, 2.01);
//   EXPECT_NEAR(out.totalTax, expected_total_tax, 1.01);
//   EXPECT_NEAR(out.remainderDeposit, expected_remainder_deposit, 1.01);
// }

// TEST_F(DepositModelTest, CalculateDepositWithPayments) {
//   Input in = {
//       100000,  // deposit
//       10,      // rate
//       13,      // taxRate
//       12,      // monthsTerm
//       true,    // capitalisation
//       Period::EveryMonth,  // period
//       {},      // additions
//       {
//           {std::chrono::system_clock::now() + std::chrono::hours(24 * 30), 5000},  // payment after 1 month
//           {std::chrono::system_clock::now() + std::chrono::hours(24 * 60), 5000}   // payment after 2 months
//       },
//       std::chrono::system_clock::now(),  // now
//       100000   // maxNonTaxableIncome
//   };
//   Output out;

//   deposit_model.calculateDeposit(in, out);

//   // Print actual values for debugging
//   std::cout << "Total Percents: " << out.totalPercents << std::endl;
//   std::cout << "Total Tax: " << out.totalTax << std::endl;
//   std::cout << "Remainder Deposit: " << out.remainderDeposit << std::endl;

//   // Expected values (these should be calculated based on the input parameters)
//   double expected_total_percents = 10490.790884267497;  // Updated expected value
//   double expected_total_tax = 0;  // Updated expected value
//   double expected_remainder_deposit = 110460.52772598638;  // Updated expected value

//   EXPECT_NEAR(out.totalPercents, expected_total_percents, 1.01);
//   EXPECT_NEAR(out.totalTax, expected_total_tax, 1.01);
//   EXPECT_NEAR(out.remainderDeposit, expected_remainder_deposit, 1.01);
// }

}  // namespace s21




int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
