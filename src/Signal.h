//
// Created by Hubert Klonowski on 11/12/2024.
//

#ifndef SIGNAL_H
#define SIGNAL_H
#include <functional>
#include <vector>

template <typename... Args>
class Signal {
public:

    using Slot = std::function<void(Args...)>;

    void connect(const Slot& slot) {
        slots.push_back(slot);
    }

    void emit(Args... args) {
        for (const auto& slot : slots) {
            slot(args...);  // Invoke the slot
        }
    }

private:
    std::vector<Slot> slots;
};

#endif //SIGNAL_H
