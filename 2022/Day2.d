import std.stdio;
import std.format;

enum Shape : char { rock = 'A', paper = 'B', scissors = 'C' }
enum Outcome : char { lose = 'X', draw = 'Y', win = 'Z' }

int outcomeScore (Outcome outcome)
{
  final switch (outcome)
    {
    case Outcome.lose: return 0;
    case Outcome.draw: return 3;
    case Outcome.win: return 6;
    }
}

int shapeScore (Shape shape)
{
  final switch (shape)
    {
    case Shape.rock: return 1;
    case Shape.paper: return 2;
    case Shape.scissors: return 3;
    }
}

Shape determineResponse (Shape opponent, Outcome outcome)
{
  Shape response;

  if ((outcome == Outcome.draw && opponent == Shape.rock) ||
      (outcome == Outcome.win && opponent == Shape.scissors) ||
      (outcome == Outcome.lose && opponent == Shape.paper))
    response = Shape.rock;
  else if ((outcome == Outcome.draw && opponent == Shape.paper) ||
           (outcome == Outcome.win && opponent == Shape.rock) ||
           (outcome == Outcome.lose && opponent == Shape.scissors))
    response = Shape.paper;
  else if ((outcome == Outcome.draw && opponent == Shape.scissors) ||
           (outcome == Outcome.win && opponent == Shape.paper) ||
           (outcome == Outcome.lose && opponent == Shape.rock))
    response = Shape.scissors;

  return response;
}

void main ()
{
  int score;
  char opponent, outcome;

  while (readf(" %s %s", opponent, outcome))
    {
      Shape response = determineResponse(cast(Shape)opponent, cast(Outcome)outcome);
      score += shapeScore(response) + outcomeScore(cast(Outcome)outcome);
    }

  writeln(score);
}
