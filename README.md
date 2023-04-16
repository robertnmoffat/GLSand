# OpenGL Sand

![Sand Image](SandScreen.png)

This project is to create something similar to the old falling sand and Powder Toy browser games. This was meant as an opportunity to exercise knowledge in OpenGL and attempting to see how many individual pixel updates I can push with reasonable performance.

## Explanation

The different particle types so far are:<br>
Sand - Falls downward and to the sides if possible<br>
Water - Falls downward. Moves horizontally if blocked downward<br>
Oil - Same as water. Ignites from fire<br>
Fire - Moves upward and to the sides in a chaotic fashion<br>
Solid - Does not move. Simply blocks other particles<br>
