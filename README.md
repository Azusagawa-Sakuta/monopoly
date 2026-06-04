# Monopoly

A multiplayer economics-themed board game built with C++20 and Qt 6 Widgets.

[![Qt Build Status](https://github.com/Azusagawa-Sakuta/monopoly/actions/workflows/main.yml/badge.svg)](https://github.com/Azusagawa-Sakuta/monopoly/actions)

[Click here to build QT!](https://github.com/Azusagawa-Sakuta/monopoly/actions/workflows/main.yml)

## Architecture

The game uses a **single-thread event-driven** architecture powered by Qt's signal/slot mechanism.

### State Machine

The game loop is driven by a `GameStep` state machine in `GameInstance`. Each tick advances through these states:

```
IDLE ──▶ ANIMATING_DICE ──▶ MOVING_PLAYER ──▶ HANDLING_TILE ──▶ TURN_END
              │                                        │
              │    ┌───────────────────────────────────┤
              │    ▼                                   ▼
              │  WAITING_BUY      WAITING_PRISON_DICE
              │    │                    │
              │    ▼                    ▼
              │  WAITING_AUCTION   WAITING_PRISON_PAYOUT
              │    │                    │
              │    ├────────────────────┤
              │    ▼                    ▼
              │  WAITING_BUILD     WAITING_SELL
              │    │                    │
              │    └────────────────────┘
              │              │
              └──────────────┘
                     │
                     ▼
              (provideInput)
                     │
                     ▼
              TURN_END ──▶ next player ──▶ IDLE
                     │
                     ▼
                 GAME_OVER
```

**Key principle:** When the game needs user input, it saves its context, sets a `Waiting*` state, emits a signal, and **returns** (does not block). The UI handles the signal, collects input, and calls `provideInput()` to resume.

### Signal/Slot Flow (Example: Buy Property)

```
GameInstance                        gameMainWidget
    │                                     │
    ├─ emit buyDecisionNeeded() ────────▶ │
    │                                     ├─ show QMessageBox
    │                                     │  "Buy this property?"
    │                                     │
    │  ◀────── provideInput(true) ────────┘
    │                                     │
    ├─ apply buy (deduct cash,            │
    │   set owner)                        │
    │                                     │
    ├─ emit propertyChanged() ──────────▶ ├─ repaint map
    │                                     │
    ├─ advance() → TurnEnd → Idle         │
    │                                     │
    ├─ emit diceRolled() ───────────────▶ ├─ enable roll button
```

### Key Classes

| Class | Role |
|-------|------|
| `GameInstance` (QObject) | Singleton managing game state, tile/player vectors, state machine. Emits signals for all events. |
| `Player` / `ComputerPlayer` | Player state (cash, position, prison status, bankruptcy). No Qt dependency. |
| `Tile` → `Buildable` / `Home` / `Prison` / `Tax` / `Random` | Board tiles with type-specific logic. No Qt dependency. |
| `gameMainWidget` | Main game UI. Connects to all `GameInstance` signals, drives display updates, collects human input. |
| `auctionWidget` | Auction dialog. Calls `provideInput()` with result. |
| `sellWidget` | Property sale dialog. Calls `provideInput()` with tiles to sell. |
| `Logger` | Singleton file logger. Single-threaded (no mutex needed). |

### Threading Model (v2.0)

- **Single thread** — Everything runs on the Qt main event loop.
- **No locks** — All `std::shared_mutex`, `std::condition_variable_any` removed.
- **No polling** — The old `QTimer` at 100ms and `QThread::sleep(1)` are replaced by direct signal/slot connections.
- **No blocking** — `waitForUserInput()` / `notifyUserInput()` replaced by `advance()` / `provideInput()` state machine.

## Game Rules

### Setup
- 2–4 players (human or computer). Configured on the Character Configuration screen.
- Each player starts with **$100,000** and chooses an avatar/color (Red, Yellow, Blue, Green).
- Board size is configurable (4–10 tiles per side, arranged in a diamond layout).

### Turn Sequence
1. **Roll dice** — Click "Roll Dice" to roll two dice. Move the sum of both dice.
2. **Doubles** — Rolling the same number on both dice grants an extra roll. Rolling the same number a third time sends you to Prison.
3. **Land on tile** — What happens depends on the tile type (see below).

### Tile Types

| Tile | Effect |
|------|--------|
| **Buildable** | Unowned: buy it ($1,000) or send to auction. Owned by another: pay rent. Owned by you: upgrade (build houses/hotel, $2,000/level). |
| **Home (GO!!)** | Pass to collect $2,500 salary. |
| **Prison** | Land here or roll three same dice → go to Prison. Three chances to roll doubles to escape, or pay to leave ($5,000 / $3,500 / $1,000). After 3 failed turns, auto-released. |
| **Tax Bureau** | Pay 10% of current cash. |
| **Casino (Random)** | Random event: teleport to Prison, teleport to Tax Bureau, teleport to GO, free upgrade, random destruction of a property, or double your money. |

### Rent
- Base rent depends on the property's development level.
- **Same-color bonus:** Rent multiplies by 1.2× for each additional property of the same color the owner holds.

### Auction
- If a player declines to buy a buildable tile, it goes to auction.
- Bidding starts at $1,000 with $500 increments. Each player gets one bid per round.

### Selling
- If you can't afford a payment, you may sell properties to the bank at **half price**.
- If you still can't afford the payment, you go bankrupt and lose.

### Winning
- The game ends when only **one player** remains solvent, or a player exceeds **$500,000**.
- Bankrupt players forfeit all properties and are removed from the game.

## Build

### Requirements
- **Qt 6** (Widgets, Core modules)
- **C++20** compiler (GCC, Clang, MSVC)
- **CMake** or **qmake**

### Build with qmake
```sh
cd frontend
qmake frontend.pro
make
```

### Build with CMake
```sh
mkdir build && cd build
cmake ..
make
```

### Run
```sh
./monopoly
# or on Windows:
monopoly.exe
```

## File Structure

```
monopoly/
├── backend/
│   ├── constant.h            # Game constants (cash, prices, tax rates)
│   ├── constant.cpp          # Constant implementations
│   ├── game.h                # Player, Tile hierarchy, GameInstance (QObject) with state machine
│   ├── game.cpp              # State machine (advance/provideInput), game logic, signal emissions
│   ├── utils.h               # Logger singleton
│   └── utils.cpp             # Logger implementation (single-threaded)
├── frontend/
│   ├── frontend.pro          # Qt project file
│   ├── main.cpp              # Entry point — QApplication only, no threads
│   ├── startwidget.h/cpp     # Start screen
│   ├── widget.h/cpp          # Player configuration/setup screen
│   ├── gamemainwidget.h      # Main game UI — signal-connected slots
│   ├── gamemainwidget.cpp    # Board rendering, player info, dice, all event handlers
│   ├── auctionwidget.h/cpp   # Auction dialog
│   ├── sellwidget.h/cpp      # Property sale dialog
│   └── resources/            # Images, GIFs (avatars, dice, tiles, houses, symbols)
├── README.md
├── Manual.pdf
└── LICENSE
```

## Gameplay Notes

- **Computer players** auto-decide on all actions (buy, build, sell, auction pass).
- The **initial cash ($100,000)** and **plot cost ($1,000)** differ from the manual's stated $10,000 — this is intentional for gameplay balance.
- The game logs all actions to timestamped `log_*.txt` files for debugging.
