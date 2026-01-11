#include "chapter3_problem46.hpp"

#include <cctype>
#include <iostream>
#include <print>

namespace SCO {
std::ostream &operator<<(std::ostream &os,
                         const std::vector<NANDGateArrayResult> &results) {
  if (results.empty())
    return os;

  // Print Header
  os << "Inputs ";
  for (size_t i = 0; i < results[0].inputs.size(); ++i)
    os << " ";
  os << "| Outputs" << std::endl;
  os << std::string(results[0].inputs.size() + results[0].outputs.size() + 10,
                    '-')
     << std::endl;

  for (const auto &res : results) {
    // Print Inputs as 0s and 1s
    for (bool in : res.inputs) {
      os << (in ? "1 " : "0 ");
    }

    os << "| ";

    // Print Outputs as 0s and 1s
    for (bool out : res.outputs) {
      os << (out ? "1 " : "0 ");
    }
    os << std::endl;
  }

  return os;
}

std::vector<NANDGateArrayResult>
simulateGateArray(std::size_t numRows, std::size_t numCols,
                  std::size_t numInputs, std::size_t numOutputs,
                  std::vector<WireConnection> wireConnections) {
  const std::size_t valuesSize = numInputs + numRows * numCols + numOutputs + 1;
  const std::size_t unusedInputIndex = valuesSize - 1;
  std::vector<NANGGateInputMapping> nandGatesInputMapping(
      numRows * numCols,
      NANGGateInputMapping{unusedInputIndex, unusedInputIndex});
  std::vector<std::size_t> outputPins(numOutputs, unusedInputIndex);

  // Create mapping
  for (const auto &[signalSource, signalDestination] : wireConnections) {
    std::size_t inputIndex = unusedInputIndex;
    if (signalSource.type == SignalSourceType::InputPin) {
      inputIndex = std::get<InputPin>(signalSource.source).index;
    } else if (signalSource.type == SignalSourceType::GateOutput) {
      auto gatePos = std::get<GateOutput>(signalSource.source).gatePos;
      inputIndex = numInputs + (gatePos.row * numCols + gatePos.col);
    }

    if (signalDestination.type == SignalDestinationType::OutputPin) {
      std::size_t outputPinIndex =
          std::get<OutputPin>(signalDestination.destination).index;
      outputPins[outputPinIndex] = inputIndex;
    } else if (signalDestination.type == SignalDestinationType::GateInput) {
      auto gatePos = std::get<GateInput>(signalDestination.destination).gatePos;
      GateInputName gateInputName =
          std::get<GateInput>(signalDestination.destination).gateInputName;
      std::size_t gateInputIndex =
          gateInputName == GateInputName::InputA ? 0 : 1;
      nandGatesInputMapping[gatePos.row * numCols + gatePos.col]
          .inputs[gateInputIndex] = inputIndex;
    }
  }

  // Simulation
  const std::size_t totalCombinations = 1 << numInputs;
  std::vector<NANDGateArrayResult> result(totalCombinations);
  for (std::size_t i = 0; i < totalCombinations; i++) {
    std::vector<bool> oldValues(numInputs + numRows * numCols + numOutputs + 1,
                                true);
    std::vector<bool> values(numInputs + numRows * numCols + numOutputs + 1,
                             true);
    bool valuesChanged = true;
    for (std::size_t bit = 0; bit < numInputs; bit++) {
      oldValues[bit] = values[bit] = (i >> bit) & 1;
      result[i].inputs.push_back(values[bit]);
    }

    // Stabilization loop
    std::print("Inputs #{}\n", i);
    for (std::size_t stabilizationRetry = 0;
         valuesChanged and
         (stabilizationRetry < nandGatesInputMapping.size() + 1);
         stabilizationRetry++) {
      std::print("Retry #{}\n", stabilizationRetry);
      for (std::size_t gatePos = 0; gatePos < nandGatesInputMapping.size();
           gatePos++) {
        auto inputAIndex = nandGatesInputMapping[gatePos].inputs[0];
        auto inputBIndex = nandGatesInputMapping[gatePos].inputs[1];
        bool inputA = oldValues[inputAIndex];
        bool inputB = oldValues[inputBIndex];
        bool output = nand(inputA, inputB);
        values[numInputs + gatePos] = output;
      }

      valuesChanged = values != oldValues;
      oldValues.swap(values);
    }

    for (std::size_t j = 0; j < outputPins.size(); j++) {
      std::size_t outputIndex = outputPins[j];
      result[i].outputs.push_back(values[outputIndex]);
    }
  }

  return result;
}
} // namespace SCO
