#include <cstdio>

struct Element {
  void insert_after(Element *new_element) {
    if (next) {
      new_element->next->prev = new_element;
    }

    new_element->next = next;
    new_element->prev = this;

    next = new_element;
  }

  void insert_before(Element *new_element) {
    if (prev) {
      new_element->prev = prev;
      new_element->next = this;

      prev->next = new_element;
      prev = new_element;
    }

    else {
      new_element->next = this;

      this->prev = new_element;
    }
  }

  Element *next{};
  Element *prev{};
  char prefix[2];
  short operating_number;
};

int main() {
  printf("Test after\n");
  // Insert after
  {
    Element trooper1, trooper2, trooper3;
    trooper1.prefix[0] = 'T';
    trooper1.prefix[1] = 'K';
    trooper1.operating_number = 421;
    trooper1.insert_after(&trooper2);
    trooper2.prefix[0] = 'F';
    trooper2.prefix[1] = 'N';
    trooper2.operating_number = 2187;
    trooper2.insert_after(&trooper3);
    trooper3.prefix[0] = 'L';
    trooper3.prefix[1] = 'S';
    trooper3.operating_number = 005;

    // Front to back
    printf("Front to back\n");
    for (Element *cursor = &trooper1; cursor; cursor = cursor->next) {
      printf("stormtrooper %c%c-%d\n", cursor->prefix[0], cursor->prefix[1],
             cursor->operating_number);
    }

    // Back to front
    printf("\nBack to front\n");
    for (Element *cursor = &trooper3; cursor; cursor = cursor->prev) {
      printf("stormtrooper %c%c-%d\n", cursor->prefix[0], cursor->prefix[1],
             cursor->operating_number);
    }
  }

  printf("\nTest before\n");
  // Insert before
  {
    Element trooper1, trooper2, trooper3;
    trooper1.prefix[0] = 'T';
    trooper1.prefix[1] = 'K';
    trooper1.operating_number = 421;

    trooper2.prefix[0] = 'F';
    trooper2.prefix[1] = 'N';
    trooper2.operating_number = 2187;

    trooper3.prefix[0] = 'L';
    trooper3.prefix[1] = 'S';
    trooper3.operating_number = 005;

    trooper3.insert_before(&trooper1);
    trooper3.insert_before(&trooper2);

    // Front to back
    printf("Front to back\n");
    for (Element *cursor = &trooper1; cursor; cursor = cursor->next) {
      printf("stormtrooper %c%c-%d\n", cursor->prefix[0], cursor->prefix[1],
             cursor->operating_number);
    }

    // Back to front
    printf("\nBack to front\n");
    for (Element *cursor = &trooper3; cursor; cursor = cursor->prev) {
      printf("stormtrooper %c%c-%d\n", cursor->prefix[0], cursor->prefix[1],
             cursor->operating_number);
    }
  }
}
