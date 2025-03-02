import random

WORDS = [
    'LEMON', 'RIFLE', 'BLISS', 'OCEAN', 'MOONS',
    'WATER', 'STONE', 'SUNNY', 'CLOUD', 'LIGHT',
    'SWIFT', 'RAVEN', 'WINDY', 'GLAZE', 'SILKY',
    'SUNUP', 'PEACH', 'HONEY', 'ROSEY', 'SLEEP',
    'DREAM', 'CHIRP', 'RAINY', 'QUIET', 'MISTY',
    'MERRY', 'HEART', 'GLOWY', 'BRITZ', 'GLOOM',
    'SMILE', 'GLOVE', 'COCOA', 'SPARK', 'GLEAM',
    'BLAZE', 'FRESH', 'PEACE', 'GUSTY', 'NOVEL',
    'BRISK', 'MAGIC', 'STARS', 'QUEEN', 'AMAZE',
    'LAUGH', 'CLEAR', 'BLOOM', 'FABLE', 'SLEEK',
    'SWEET', 'GREEN', 'FLUFF', 'FLAME', 'QUICK',
    'FANCY', 'SHINE', 'SMALL', 'SOLID', 'SHARP',
    'SOILS', 'WRING', 'ZESTY', 'MINTY', 'ROUGE',
    'BLUSH', 'WASTE', 'VALID', 'CLAIM', 'DIRTY',
    'EAGER', 'FEAST', 'HAPPY', 'JADED', 'KITES',
    'LUCKY', 'NOBLE', 'OPALS', 'PINKY', 'QUIRK',
    'ROYAL', 'SASSY', 'TANGO', 'UNITY', 'VALOR',
    'CROWN', 'GRAPE', 'PLUMB', 'MELON', 'THYME',
    'CLOVE', 'SPICE', 'FROST', 'SLEET', 'FLUTE',
    'TULIP', 'BLITZ', 'SMIRK', 'SKIRT', 'TRICK'
]

WORD_LENGTH = 5
MAX_GUESSES = 4


def generate_random_word():
    return random.choice(WORDS)


def check_guess(target_word, guess):
    correct_positions = []
    for c1, c2 in zip(target_word, guess):
        if c1 == c2:
            print(f"Letter {c1} is in the correct position")
            correct_positions.append(c1)
    return correct_positions


def check_presence(guess, target_word, correct_positions):
    for letter in guess:
        if letter in target_word and letter not in correct_positions:
            print(f"Letter '{letter}' is present in the target word")


def play_wordle():
    target_word = generate_random_word()
    print("Welcome to Wordle!")
    print(f"Guess the {WORD_LENGTH}-letter word.")
    print(f"You have {MAX_GUESSES} attempts.")

    guesses = 0
    while guesses < MAX_GUESSES:
        guess = input("Enter your guess: ").upper()

        if guess == target_word:
            print("Congratulations! You guessed the word correctly!")
            return

        correct_positions = check_guess(target_word, guess)
        check_presence(guess, target_word, correct_positions)

        guesses += 1

    print(f"Sorry, you couldn't guess the word. The word was {target_word}.")


play_wordle()
