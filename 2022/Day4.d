import std.stdio;

struct SectionAssignment
{
  int start, end;

  bool containedIn(SectionAssignment b)
  {
    return start >= b.start && end <= b.end;
  }

  bool overlaps(SectionAssignment b)
  {
    return containedIn(b) || (start <= b.end && start >= b.start) || (end <= b.end && end >= b.start);
  }
}

void main ()
{
  int count;
  SectionAssignment[2] assignmentPair; 

  while (readf(" %s-%s,%s-%s", assignmentPair[0].start, assignmentPair[0].end,
               assignmentPair[1].start, assignmentPair[1].end))
    {
      count += assignmentPair[0].overlaps(assignmentPair[1]) ||
        assignmentPair[1].overlaps(assignmentPair[0]);
    }
  writeln(count);
}
