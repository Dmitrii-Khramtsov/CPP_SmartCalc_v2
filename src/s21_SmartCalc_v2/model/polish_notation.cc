#include "polish_notation.h"

#include <iostream>

namespace s21 {

/******************************************************************************
 * MAIN METHODS
 ******************************************************************************/

/**
 * @brief Converts an infix expression to Reverse Polish Notation (RPN).
 *
 * @param infix The infix expression to be converted.
 * @return The RPN expression.
 */
String ReversePolishNotation::toRPN(const String& infix) {
  String processed_infix = processedInfix(infix);
  StringStack operators;  // стек для хранения операторов и функций
  String output;  // строка для хранения результата в RPN
  String token;  // временная строка для накопления операндов

  for (size_t i = 0; i < processed_infix.length(); ++i) {
    switch (isOperandOrOperatorOrFunction(processed_infix, i)) {
      case OPERAND:
        handleOperand(output, token, processed_infix, i);
        break;
      case OPERATOR:
        processed_infix[i] == '~'
            ? handleUnaryMinus(output, token, operators)
            : handleOperator(output, token, operators, processed_infix[i]);
        break;
      case FUNCTION:
        handleFunction(output, token, operators, processed_infix[i]);
        break;
      case BRACKETS:
        handleParenthesis(output, token, operators, processed_infix[i]);
        break;
    }
  }

  flushToken(output, token);
  flushOperators(operators, output);

  return trim(output);
}

/******************************************************************************
 * AUXILIARY PRIVATE MAIN METHODS
 ******************************************************************************/

/**
 * @brief Map of operator priorities.
 */
const OperatorPriorityMap
    ReversePolishNotation::operatorPriority = {
        {'(', OP_PARENTHESIS},    {')', OP_PARENTHESIS},
        {'+', OP_ADDITIVE},       {'-', OP_ADDITIVE},
        {'*', OP_MULTIPLICATIVE}, {'/', OP_MULTIPLICATIVE},
        {'%', OP_MULTIPLICATIVE}, {'s', OP_FUNCTION},
        {'c', OP_FUNCTION},       {'t', OP_FUNCTION},
        {'i', OP_FUNCTION},       {'o', OP_FUNCTION},
        {'n', OP_FUNCTION},       {'q', OP_FUNCTION},
        {'l', OP_FUNCTION},       {'g', OP_FUNCTION},
        {'^', OP_EXPONENTIAL},    {'~', OP_UNARY_MINUS},
};

/**
 * @brief Map of function names to their corresponding characters.
 */
const FunctionMap ReversePolishNotation::functions = {
    {"sin", 's'},  {"cos", 'c'},  {"tan", 't'}, {"asin", 'i'}, {"acos", 'o'},
    {"atan", 'n'}, {"sqrt", 'q'}, {"ln", 'l'},  {"log", 'g'}};

/**
 * @brief Checks if a character represents a function.
 *
 * @param fn The character to be checked.
 * @return True if the character represents a function, false otherwise.
 */
bool ReversePolishNotation::isFunction(const char fn) {
  static const std::unordered_set<char> functions = {'s', 'c', 't', 'i', 'o',
                                                     'n', 'q', 'l', 'g'};
  return functions.find(fn) != functions.end();
}

/**
 * @brief Gets the priority of an operator.
 *
 * @param op The operator to be checked.
 * @return The priority of the operator.
 */
int ReversePolishNotation::getOperatorPriority(const char op) {
  auto it = operatorPriority.find(op);
  if (it != operatorPriority.end()) {
    return it->second;  // если существует - возвращаем приоритет
  }
  throw std::invalid_argument("Unknown operator");
}

/**
 * @brief Checks if a character is an operand (digit, dot, or 'x').
 *
 * @param c The character to be checked.
 * @return True if the character is an operand, false otherwise.
 */
bool ReversePolishNotation::isOperand(const char c) {
  return std::isdigit(c) || c == '.' || c == 'x';
}

/**
 * @brief Checks if a character is an operator.
 *
 * @param op The character to be checked.
 * @return True if the character is an operator, false otherwise.
 */
bool ReversePolishNotation::isOperator(const char op) {
  static const std::unordered_set<char> functions = {'+', '-', '*', '/',
                                                     '^', '%', '~'};
  return functions.find(op) != functions.end();
}

/**
 * @brief Determines if a character is an operand, operator, or function.
 *
 * @param infix The infix expression.
 * @param i The index of the character in the expression.
 * @return The type of the character.
 */
int ReversePolishNotation::isOperandOrOperatorOrFunction(
    const String& infix, size_t i) {
  char c = infix[i];

  if (isOperand(c)) return OPERAND;
  if (isOperator(c)) return OPERATOR;
  if (isFunction(c)) return FUNCTION;
  return BRACKETS;
}

/**
 * @brief Adds a complete operand to the output string.
 *
 * @param output The output string.
 * @param token The token to be added.
 */
void ReversePolishNotation::processOperand(String& output,
                                           const String& token) {
  output += token + " ";
}

/**
 * @brief Pops and appends the top operator from the stack to the output string.
 *
 * @param operators The stack of operators.
 * @param output The output string.
 */
void ReversePolishNotation::popAndAppendOperator(
    StringStack& operators, String& output) {
  if (!operators.empty()) {
    output += operators.top() + " ";
    operators.pop();
  }
}

/**
 * @brief Adds all remaining operators in the stack to the output string.
 *
 * @param operators The stack of operators.
 * @param output The output string.
 */
void ReversePolishNotation::flushOperators(StringStack& operators,
                                           String& output) {
  while (!operators.empty()) {
    popAndAppendOperator(operators, output);
  }
}

/**
 * @brief Renames functions in the infix expression.
 *
 * @param infix The infix expression.
 * @return The modified infix expression with renamed functions.
 */
String ReversePolishNotation::renameFunctions(const String& infix) {
  String result = infix;

  for (const auto& pair : functions) {
    size_t pos = 0;
    while ((pos = result.find(pair.first, pos)) != String::npos) {
      result.replace(pos, pair.first.length(), String(1, pair.second));
      pos += 1;
    }
  }

  return result;
}

/**
 * @brief Replaces unary minus with a special character in the infix expression.
 *
 * @param infix The infix expression.
 * @return The modified infix expression with replaced unary minus.
 */
String ReversePolishNotation::replaceUnaryMinus(const String& infix) {
  String result = infix;

  for (size_t i = 0; i < result.length(); ++i) {
    if (result[i] == '-' && isUnaryMinus(result, i)) {
      result[i] = '~';
    }
  }

  return result;
}

/**
 * @brief Processes the infix expression by renaming functions
 * and replacing unary minus '~'.
 *
 * @param infix The infix expression.
 * @return The processed infix expression.
 */
String ReversePolishNotation::processedInfix(const String& infix) {
  String result = renameFunctions(infix);
  result = replaceUnaryMinus(result);

  return result;
}

/**
 * @brief Adds any remaining token to the output string.
 *
 * @param output The output string.
 * @param token The token to be added.
 */
void ReversePolishNotation::flushToken(String& output,
                                       String& token) {
  if (!token.empty()) {
    processOperand(output, token);
    token.clear();
  }
}

/**
 * @brief Checks if a character is part of an exponent in scientific notation.
 *
 * @param c The character to be checked.
 * @return True if the character is part of an exponent, false otherwise.
 */
bool ReversePolishNotation::isPartOfExponent(const char c) {
  return c == 'e' || c == 'E';
}

/**
 * @brief Handles individual characters that are part of an operand,
 * deciding whether to add them to the current operand token
 * or process the current token.
 *
 * @param output The output string.
 * @param token The current operand token.
 * @param infix The infix expression.
 * @param i The index of the current character in the expression.
 */
void ReversePolishNotation::handleOperand(String& output,
                                          String& token,
                                          const String& infix, size_t& i) {
  if (isOperand(infix[i])) {
    findScientificNumber(infix, i, token);
    flushToken(output, token);
  } else {
    flushToken(output, token);
  }
}

/**
 * @brief Checks if a minus sign is a unary minus.
 *
 * @param infix The infix expression.
 * @param i The index of the minus sign in the expression.
 * @return True if the minus sign is a unary minus, false otherwise.
 */
bool ReversePolishNotation::isUnaryMinus(const String& infix, size_t i) {
  if (i == 0 || infix[i - 1] == '(' ||
      (isOperator(infix[i - 1]) && infix[i - 1] != 'e' &&
       infix[i - 1] != 'E')) {
    return true;
  }
  return false;
}

/**
 * @brief Handles a unary minus.
 *
 * @param output The output string.
 * @param token The current operand token.
 * @param operators The stack of operators.
 */
void ReversePolishNotation::handleUnaryMinus(
    String& output, String& token,
    StringStack& operators) {
  flushToken(output, token);
  operators.push(String(1, '~'));  // используем '~' для унарного минуса
}

/**
 * @brief Handles a binary operator.
 *
 * @param output The output string.
 * @param token The current operand token.
 * @param operators The stack of operators.
 * @param c The operator character.
 */
void ReversePolishNotation::handleOperator(String& output,
                                           String& token,
                                           StringStack& operators,
                                           char c) {
  flushToken(output, token);
  while (!operators.empty()) {
    char topOperator = operators.top()[0];
    int topPriority = getOperatorPriority(topOperator);
    int currentPriority = getOperatorPriority(c);
    if (topPriority >= currentPriority) {
      popAndAppendOperator(operators, output);
    } else {
      break;
    }
  }
  operators.push(String(1, c));
}

/**
 * @brief Handles a function.
 *
 * @param output The output string.
 * @param token The current operand token.
 * @param operators The stack of operators.
 * @param c The function character.
 */
void ReversePolishNotation::handleFunction(String& output,
                                           String& token,
                                           StringStack& operators,
                                           char c) {
  flushToken(output, token);
  operators.push(String(1, c));
}

/**
 * @brief Handles parentheses.
 *
 * @param output The output string.
 * @param token The current operand token.
 * @param operators The stack of operators.
 * @param c The parenthesis character.
 */
void ReversePolishNotation::handleParenthesis(
    String& output, String& token, StringStack& operators,
    char c) {
  if (c == '(') {
    operators.push(String(1, c));  // перенести открывающую скобку в стек
  } else if (c == ')') {
    flushToken(output, token);
    while (!operators.empty() && operators.top() != "(") {
      popAndAppendOperator(operators, output);
    }
    if (!operators.empty() && operators.top() == "(") {
      operators.pop();  // вытащить открывающую скобку
    }
  }
}

/**
 * @brief Processes the exponent part of a scientific number.
 *
 * @param infix The infix expression.
 * @param i The index of the current character in the expression.
 * @param token The current operand token.
 */
void ReversePolishNotation::processExponent(const String& infix, size_t& i,
                                            String& token) {
  if (i + 1 < infix.size() && (infix[i + 1] == '+' || infix[i + 1] == '-')) {
    token += infix[++i];  // добавляем '+' или '-' убедимся,
    // что после '+' или '-' идет число
    if (i + 1 < infix.size() && std::isdigit(infix[i + 1])) {
      token += infix[++i];
    } else {
      // если после '+' или '-' не следует число, выводим ошибку
      throw std::invalid_argument(
          "Invalid format: expected digit after 'e' and sign");
    }
  } else if (i + 1 < infix.size() && std::isdigit(infix[i + 1])) {
    token += infix[++i];
  } else {
    // если после 'e' или 'E' не следует число, выводим ошибку
    throw std::invalid_argument("Invalid format: expected digit after 'e'");
  }
}

/**
 * @brief Finds a scientific number in the infix expression.
 *
 * @param infix The infix expression.
 * @param i The index of the current character in the expression.
 * @param token The current operand token.
 */
void ReversePolishNotation::findScientificNumber(const String& infix,
                                                 size_t& i,
                                                 String& token) {
  bool hasExponent = false;
  while (i < infix.size() &&
         (isOperand(infix[i]) || isPartOfExponent(infix[i]))) {
    token += infix[i];
    if ((isPartOfExponent(infix[i])) && !hasExponent) {
      hasExponent = true;
      processExponent(infix, i, token);
    }
    i++;
  }
  i--;  // компенсируем инкремент i на последнем шаге
}

/**
 * @brief Trims leading and trailing spaces from a string.
 * (удаляет начальные и конечные пробелы из строки)
 *
 * @param str The string to be trimmed.
 * @return The trimmed string.
 */
String ReversePolishNotation::trim(const String& str) {
  size_t first = str.find_first_not_of(' ');
  if (first == String::npos) return "";  // нет контента
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

}  // namespace s21
