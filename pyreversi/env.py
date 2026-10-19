import gym
from gym import spaces
import numpy as np
import reversi_gym

class ReversiEnv(gym.Env):
    metadata = {'render.modes': ['human']}

    def __init__(self):
        super(ReversiEnv, self).__init__()
        # Board size is 8x8, values are WHITE(1), BLACK(-1), EMPTY(0)
        self.observation_space = spaces.Box(low=-1, high=1, shape=(8, 8), dtype=np.int32)
        # Action space is 8x8 = 64
        self.action_space = spaces.Discrete(64)
        self.game = reversi_gym.Game()

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        self.game = reversi_gym.Game()
        board = self.game.getboard()
        return board.get_state(), {}

    def step(self, action):
        x = action // 8
        y = action % 8
        move = reversi_gym.Move(x, y)

        board = self.game.getboard()

        if not board.isvalidmove(move):
            # Invalid move
            return board.get_state(), -10.0, True, False, {"error": "Invalid move"}

        # Player makes move
        self.game.playerinput(move)

        # In Reversi, the other player should make a move.
        # But for gym environment, the agent might play both sides or against a built-in AI.
        # Let's let the environment allow playing against the built-in AI for simplicity if specified,
        # or we just let it take turns. We'll just let the environment take one turn per step.

        if self.game.isgameover():
            winner = self.game.getwinner()
            reward = 0.0
            if winner == reversi_gym.BLACK:  # Black went first, typically agent
                reward = 1.0
            elif winner == reversi_gym.WHITE:
                reward = -1.0
            return self.game.getboard().get_state(), reward, True, False, {}

        board = self.game.getboard()
        return board.get_state(), 0.0, self.game.isgameover(), False, {}

    def render(self, mode='human'):
        board = self.game.getboard().get_state()
        print(board)
