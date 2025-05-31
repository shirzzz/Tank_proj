import pygame
import sys
import re

# ----- CONFIG -----
CELL_SIZE = 60
FPS = 1  # frames per second (controls speed)
# ------------------

# These will be set when we read map.txt
GRID_WIDTH = None
GRID_HEIGHT = None


# ----- TANK & SHELL CLASSES -----
class Tank:
    def __init__(self, player_id, tank_index, x, y, images_dict):
        """
        player_id: 1 or 2
        tank_index: index within that player (0, 1, 2, ...)
        x, y: initial grid cell (integers)
        images_dict: dict of loaded pygame Surfaces for each (player, direction) key
        """
        self.player_id = player_id
        self.tank_index = tank_index
        self.x = x
        self.y = y
        # Default direction: player 1 faces LEFT, player 2 faces RIGHT
        if player_id == 1:
            self.direction = 'LEFT'
        else:
            self.direction = 'RIGHT'
        self.alive = True
        self.images = images_dict  # reference to the global images

    def move_forward(self):
        if not self.alive:
            return
        if self.direction == 'UP':
            self.y = (self.y - 1) % GRID_HEIGHT
        elif self.direction == 'DOWN':
            self.y = (self.y + 1) % GRID_HEIGHT
        elif self.direction == 'LEFT':
            self.x = (self.x - 1) % GRID_WIDTH
        elif self.direction == 'RIGHT':
            self.x = (self.x + 1) % GRID_WIDTH
        elif self.direction == 'UP_RIGHT':
            self.x = (self.x + 1) % GRID_WIDTH
            self.y = (self.y - 1) % GRID_HEIGHT
        elif self.direction == 'UP_LEFT':
            self.x = (self.x - 1) % GRID_WIDTH
            self.y = (self.y - 1) % GRID_HEIGHT
        elif self.direction == 'DOWN_RIGHT':
            self.x = (self.x + 1) % GRID_WIDTH
            self.y = (self.y + 1) % GRID_HEIGHT
        elif self.direction == 'DOWN_LEFT':
            self.x = (self.x - 1) % GRID_WIDTH
            self.y = (self.y + 1) % GRID_HEIGHT
        else:
            raise ValueError(f"Unknown direction: {self.direction}")

    def move_backward(self):
        if not self.alive:
            return
        if self.direction == 'UP':
            self.y = (self.y + 1) % GRID_HEIGHT
        elif self.direction == 'DOWN':
            self.y = (self.y - 1) % GRID_HEIGHT
        elif self.direction == 'LEFT':
            self.x = (self.x + 1) % GRID_WIDTH
        elif self.direction == 'RIGHT':
            self.x = (self.x - 1) % GRID_WIDTH
        elif self.direction == 'UP_RIGHT':
            self.x = (self.x - 1) % GRID_WIDTH
            self.y = (self.y + 1) % GRID_HEIGHT
        elif self.direction == 'UP_LEFT':
            self.x = (self.x + 1) % GRID_WIDTH
            self.y = (self.y + 1) % GRID_HEIGHT
        elif self.direction == 'DOWN_RIGHT':
            self.x = (self.x - 1) % GRID_WIDTH
            self.y = (self.y - 1) % GRID_HEIGHT
        elif self.direction == 'DOWN_LEFT':
            self.x = (self.x + 1) % GRID_WIDTH
            self.y = (self.y - 1) % GRID_HEIGHT
        else:
            raise ValueError(f"Unknown direction: {self.direction}")

    def rotate_left45(self):
        if not self.alive:
            return
        if self.direction == 'UP':
            self.direction = 'UP_LEFT'
        elif self.direction == 'UP_LEFT':
            self.direction = 'LEFT'
        elif self.direction == 'LEFT':
            self.direction = 'DOWN_LEFT'
        elif self.direction == 'DOWN_LEFT':
            self.direction = 'DOWN'
        elif self.direction == 'DOWN':
            self.direction = 'DOWN_RIGHT'
        elif self.direction == 'DOWN_RIGHT':
            self.direction = 'RIGHT'
        elif self.direction == 'RIGHT':
            self.direction = 'UP_RIGHT'
        elif self.direction == 'UP_RIGHT':
            self.direction = 'UP'
        else:
            raise ValueError(f"Unknown direction: {self.direction}")

    def rotate_right45(self):
        if not self.alive:
            return
        if self.direction == 'UP':
            self.direction = 'UP_RIGHT'
        elif self.direction == 'UP_RIGHT':
            self.direction = 'RIGHT'
        elif self.direction == 'RIGHT':
            self.direction = 'DOWN_RIGHT'
        elif self.direction == 'DOWN_RIGHT':
            self.direction = 'DOWN'
        elif self.direction == 'DOWN':
            self.direction = 'DOWN_LEFT'
        elif self.direction == 'DOWN_LEFT':
            self.direction = 'LEFT'
        elif self.direction == 'LEFT':
            self.direction = 'UP_LEFT'
        elif self.direction == 'UP_LEFT':
            self.direction = 'UP'
        else:
            raise ValueError(f"Unknown direction: {self.direction}")

    def rotate_left90(self):
        if not self.alive:
            return
        if self.direction == 'UP':
            self.direction = 'LEFT'
        elif self.direction == 'LEFT':
            self.direction = 'DOWN'
        elif self.direction == 'DOWN':
            self.direction = 'RIGHT'
        elif self.direction == 'RIGHT':
            self.direction = 'UP'
        elif self.direction == 'UP_RIGHT':
            self.direction = 'UP_LEFT'
        elif self.direction == 'UP_LEFT':
            self.direction = 'DOWN_LEFT'
        elif self.direction == 'DOWN_LEFT':
            self.direction = 'DOWN_RIGHT'
        elif self.direction == 'DOWN_RIGHT':
            self.direction = 'UP_RIGHT'
        else:
            raise ValueError(f"Unknown direction: {self.direction}")

    def rotate_right90(self):
        if not self.alive:
            return
        if self.direction == 'UP':
            self.direction = 'RIGHT'
        elif self.direction == 'RIGHT':
            self.direction = 'DOWN'
        elif self.direction == 'DOWN':
            self.direction = 'LEFT'
        elif self.direction == 'LEFT':
            self.direction = 'UP'
        elif self.direction == 'UP_RIGHT':
            self.direction = 'DOWN_RIGHT'
        elif self.direction == 'DOWN_RIGHT':
            self.direction = 'DOWN_LEFT'
        elif self.direction == 'DOWN_LEFT':
            self.direction = 'UP_LEFT'
        elif self.direction == 'UP_LEFT':
            self.direction = 'UP_RIGHT'
        else:
            raise ValueError(f"Unknown direction: {self.direction}")

    def shoot(self):
        """
        Spawn a Shell object in front of the tank.
        Returns a new Shell instance or None if the tank is dead.
        """
        if not self.alive:
            return None

        if self.direction == 'UP':
            sx, sy = self.x, self.y - 1
            dx, dy = 0, -1
        elif self.direction == 'DOWN':
            sx, sy = self.x, self.y + 1
            dx, dy = 0, 1
        elif self.direction == 'LEFT':
            sx, sy = self.x - 1, self.y
            dx, dy = -1, 0
        elif self.direction == 'RIGHT':
            sx, sy = self.x + 1, self.y
            dx, dy = 1, 0
        elif self.direction == 'UP_RIGHT':
            sx, sy = self.x + 1, self.y - 1
            dx, dy = 1, -1
        elif self.direction == 'UP_LEFT':
            sx, sy = self.x - 1, self.y - 1
            dx, dy = -1, -1
        elif self.direction == 'DOWN_RIGHT':
            sx, sy = self.x + 1, self.y + 1
            dx, dy = 1, 1
        elif self.direction == 'DOWN_LEFT':
            sx, sy = self.x - 1, self.y + 1
            dx, dy = -1, 1
        else:
            return None

        sx %= GRID_WIDTH
        sy %= GRID_HEIGHT
        return Shell(sx, sy, dx, dy)

    def get_sprite(self):
        """Return the correct image for this tank’s player and current direction."""
        direction_key = self.direction.lower().replace('_', '')
        key = f"player{self.player_id}_{direction_key}"
        return self.images[key]


class Shell:
    def __init__(self, x, y, dx, dy):
        """
        x, y: starting cell coordinates (integers)
        dx, dy: velocity in cells/frame (each of -1, 0, +1)
        """
        self.x = x
        self.y = y
        self.dx = dx
        self.dy = dy
        self.radius = CELL_SIZE // 6

    def update_position(self):
        """Move the shell one step in (dx, dy) and wrap around the grid."""
        self.x = (self.x + self.dx) % GRID_WIDTH
        self.y = (self.y + self.dy) % GRID_HEIGHT

    def draw(self, screen):
        """Convert grid pos (self.x, self.y) to pixel pos and draw a circle."""
        px = self.x * CELL_SIZE + CELL_SIZE // 2
        py = self.y * CELL_SIZE + CELL_SIZE // 2
        pygame.draw.circle(screen, (200, 30, 30), (px, py), self.radius)


# ----- UTILITY FUNCTIONS -----
def draw_grid(screen):
    """Draw a faint gray grid background (fill + borders)."""
    for y in range(GRID_HEIGHT):
        for x in range(GRID_WIDTH):
            rect = pygame.Rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE)
            # Fill with light gray first
            pygame.draw.rect(screen, (230, 230, 230), rect)
            # Then draw the border
            pygame.draw.rect(screen, (100, 100, 100), rect, 1)


def draw_map(screen, grid, images):
    """
    Draw walls (#), mines (@), and empty spaces. Should be called AFTER draw_grid.
    """
    for y, row in enumerate(grid):
        for x, cell in enumerate(row):
            rect = pygame.Rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE)
            if cell == '#':
                screen.blit(images['wall'], rect)
            elif cell == '@':
                screen.blit(images['mine'], rect)
            else:
                screen.blit(images['empty'], rect)


def draw_tanks(screen, tanks):
    """For each tank, if alive, blit its sprite at (x, y)."""
    for tank in tanks:
        if not tank.alive:
            continue
        sprite = tank.get_sprite()
        screen.blit(sprite, (tank.x * CELL_SIZE, tank.y * CELL_SIZE))


def draw_shells(screen, shells):
    """Draw each Shell's circle at its current (x, y)."""
    for shell in shells:
        shell.draw(screen)


def clean_action(token):
    """
    Remove any parenthetical notes (e.g., "(ignored)", "(killed)") and strip whitespace.
    If the result is an empty string, return "DoNothing".
    """
    cleaned = re.sub(r'\s*\(.*?\)', '', token).strip()
    return cleaned if cleaned else "DoNothing"


def parse_action(action_str, tank):
    """
    Given a single action token (e.g., "MoveForward", "RotateLeft45", "Shoot", or "Killed"),
    mutate the tank (or return a new Shell). If the tank is “killed”, mark it as dead.
    Returns a Shell instance if action_str == "Shoot", otherwise returns None.
    """
    if not tank.alive:
        return None

    action = action_str.strip().lower()
    if action == "killed":
        tank.alive = False
        return None
    elif action == "moveforward":
        tank.move_forward()
    elif action == "movebackward":
        tank.move_backward()
    elif action == "rotateleft45":
        tank.rotate_left45()
    elif action == "rotateright45":
        tank.rotate_right45()
    elif action == "rotateleft90":
        tank.rotate_left90()
    elif action == "rotateright90":
        tank.rotate_right90()
    elif action == "shoot":
        return tank.shoot()
    elif action == "donothing" or action == "getbattleinfo":
        pass
    else:
        pass
    return None


# ----- IMAGE LOADING -----
def load_images():
    """
    Load all images from disk, scale them to (CELL_SIZE, CELL_SIZE),
    and store in a dict keyed by:
       'player1_up', 'player1_down', 'player1_left', 'player1_right', etc.
       'player2_up', etc.
       'wall', 'mine', 'empty'
    """
    raw_names = [
        'player1_up', 'player1_down', 'player1_left', 'player1_right',
        'player1_upright', 'player1_upleft', 'player1_downright', 'player1_downleft',
        'player2_up', 'player2_down', 'player2_left', 'player2_right',
        'player2_upright', 'player2_upleft', 'player2_downright', 'player2_downleft',
        'wall', 'mine', 'empty'
    ]
    imgs = {}
    for name in raw_names:
        path = f'images/{name}.png'
        surf = pygame.image.load(path).convert_alpha()
        surf = pygame.transform.scale(surf, (CELL_SIZE, CELL_SIZE))
        imgs[name] = surf
    return imgs


# ----- MAP LOADING -----
def load_map_dimensions():
    """
    Reads the first 5 nonblank lines of map.txt, which look like:
      <anything>
      MaxSteps = <NUM>
      NumShells = <NUM>
      Rows = <NUM>
      Cols = <NUM>
    We pull out 'Rows' and 'Cols' and set the globals GRID_WIDTH/GRID_HEIGHT.
    """
    global GRID_WIDTH, GRID_HEIGHT
    with open("map.txt", "r") as f:
        lines = [line.strip() for line in f if line.strip()]
    if len(lines) < 5:
        raise ValueError("map.txt must have at least 5 nonblank lines.")

    for line in lines[:5]:
        if line.startswith("Rows"):
            GRID_HEIGHT = int(line.split('=')[1].strip())
        elif line.startswith("Cols"):
            GRID_WIDTH = int(line.split('=')[1].strip())

    if GRID_WIDTH is None or GRID_HEIGHT is None:
        raise ValueError("Failed to parse Rows/Cols from map.txt")


def load_map():
    """
    After reading dimensions, read lines 6 onward of map.txt, pad or truncate 
    each line to GRID_WIDTH, and return a list of rows (each row is a list of chars).
    """
    with open("map.txt", "r") as f:
        raw_lines = [line.rstrip("\n") for line in f]

    content_lines = raw_lines[5:]  # skip header
    grid = []
    for r in range(GRID_HEIGHT):
        if r < len(content_lines):
            line = content_lines[r]
            if len(line) < GRID_WIDTH:
                line = line + " " * (GRID_WIDTH - len(line))
            line = line[:GRID_WIDTH]
        else:
            line = " " * GRID_WIDTH
        grid.append(list(line))
    return grid


# ----- MAIN VISUALIZER -----
def main():
    pygame.init()
    load_map_dimensions()
    grid = load_map()

    # Set up screen:
    screen_width = GRID_WIDTH * CELL_SIZE
    screen_height = GRID_HEIGHT * CELL_SIZE
    screen = pygame.display.set_mode((screen_width, screen_height))
    pygame.display.set_caption("Tank Game Visualizer")
    clock = pygame.time.Clock()

    # Load all images once:
    images = load_images()

    # Dynamically spawn tanks by scanning the grid for digit characters.
    tanks = []
    counts = {1: 0, 2: 0}
    for row in range(GRID_HEIGHT):
        for col in range(GRID_WIDTH):
            c = grid[row][col]
            if c.isdigit():
                num = int(c)
                player_id = 1 if (num % 2) == 1 else 2
                tank_index = counts[player_id]
                counts[player_id] += 1
                tanks.append(
                    Tank(player_id=player_id,
                         tank_index=tank_index,
                         x=col,
                         y=row,
                         images_dict=images)
                )

    # Maintain a list of active shells:
    shells = []

    # Read actions from output.txt
    action_log = []
    with open("output.txt", "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            # If we reach a line indicating who won, stop parsing rounds
            if line.lower().startswith("player"):
                break
            # Split by commas, clean parentheses annotations, and strip whitespace
            tokens = [token.strip() for token in line.split(",")]
            cleaned_round = [clean_action(token) for token in tokens]
            action_log.append(cleaned_round)

    current_round = 0
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # --- Apply one round of actions (if any remain) ---
        if current_round < len(action_log):
            action_round = action_log[current_round]
            for i, tank in enumerate(tanks):
                if i < len(action_round):
                    new_shell = parse_action(action_round[i], tank)
                else:
                    new_shell = parse_action("DoNothing", tank)
                if new_shell is not None:
                    shells.append(new_shell)
            current_round += 1

        # --- Advance all shells one step & detect collisions ---
        for shell in shells[:]:
            shell.update_position()
            tx, ty = shell.x, shell.y

            # Check collision with wall
            if grid[ty][tx] == '#':
                shells.remove(shell)
                continue

            # Check collision with any tank
            for tank in tanks:
                if tank.alive and tank.x == tx and tank.y == ty:
                    tank.alive = False
                    if shell in shells:
                        shells.remove(shell)
                    break

        # Now draw everything in the correct order:
        screen.fill((255, 255, 255))

        # 1) Draw the grid background first
        draw_grid(screen)

        # 2) Draw walls (#), mines (@), and empty spaces
        draw_map(screen, grid, images)

        # 3) Draw tanks on top of walls/mines
        draw_tanks(screen, tanks)

        # 4) Draw any active shells on top of tanks
        draw_shells(screen, shells)

        pygame.display.flip()
        clock.tick(FPS)

    pygame.quit()
    sys.exit()


if __name__ == "__main__":
    main()
