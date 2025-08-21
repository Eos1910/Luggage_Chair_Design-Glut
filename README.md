# Luggage_Chair_Design-Glut

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <h1>VoyagePal - 3D Luggage Chair</h1>
</head>
<body>
  <p>
    A 3D interactive model of a multi-functional luggage chair developed using
    <strong>C++</strong>, <strong>OpenGL</strong>, and <strong>GLUT</strong>.
    Designed to improve travel convenience, VoyagePal features a foldable seat,
    internal charger, dynamic textures, and user-controlled animations.
  </p>

  <h2>🌟 Features</h2>
  <ul>
    <li>3D visualization of a smart luggage + foldable chair combo</li>
    <li>Realistic lighting and shadows</li>
    <li>Interactive Type-C charger lid</li>
    <li>Foldable chair animation with support rods</li>
    <li>Functional zipper mechanics</li>
    <li>Image & procedural textures (via <code>stb_image.h</code>)</li>
    <li>Keyboard and mouse input for real-time control</li>
    <li>Smooth animations using <code>glutTimerFunc</code></li>
  </ul>

  <h2>🎮 Controls</h2>
  <table border="1" cellpadding="6">
    <thead>
      <tr><th>Key</th><th>Action</th></tr>
    </thead>
    <tbody>
      <tr><td>F1–F8</td><td>Toggle suitcase features (handle, chair, etc.)</td></tr>
      <tr><td>← ↑ ↓ →</td><td>Rotate scene</td></tr>
      <tr><td>R / G / Y</td><td>Change luggage color</td></tr>
      <tr><td>P</td><td>Toggle charger port lid</td></tr>
      <tr><td>Mouse</td><td>Open/close zipper & lid</td></tr>
    </tbody>
  </table>

  <h2>🛠️ Tech Stack</h2>
  <ul>
    <li>C++</li>
    <li>OpenGL</li>
    <li>GLUT</li>
    <li><code>stb_image.h</code> for texture support</li>
  </ul>

  <h2>📦 How to Run</h2>
  <ol>
    <li>Install Code::Blocks</li>
    <li>
      Follow this GLUT setup guide for Code::Blocks:<br>
      <a href="https://github.com/Goblin80/glut-install" target="_blank">
        GLUT Setup Guide on GitHub
      </a>
    </li>
    <li>Clone this repository:
      <pre><code>git clone https://github.com/your-username/voyagepal.git</code></pre>
    </li>
    <li>Open the project in Code::Blocks, build, and run</li>
  </ol>

  <h2>📁 Project Structure</h2>
  <pre>
|-- main.cpp
|-- textures/
|   |-- metal.jpg
|   |-- logo.png
|-- shaders/
|-- stb_image.h
|-- README.md
  </pre>

  <h2>📷 Screenshots</h2>
  <p>
    (Add images of key states: zipped, opened, chair deployed, texture details)
  </p>

  <h2>👨‍💻 Contributors</h2>
  <ul>
    <li><strong>Thet Paing Soe</strong> (CST2209301)</li>
    <li><strong>Bryan Winata</strong> (CST2209199)</li>
    <li><strong>Zhang Siyu</strong> (CST2209178)</li>
  </ul>

  <h2>📚 Learning Highlights</h2>
  <ul>
    <li>Practiced 3D object modeling with OpenGL</li>
    <li>Implemented animation using timers and input</li>
    <li>Learned basic texture and lighting techniques</li>
    <li>Simulated real-world interactions programmatically</li>
  </ul>

  <h2>📖 References</h2>
  <ul>
    <li><a href="https://psgraphics.blogspot.com/2015/06/a-small-image-io-library-stbimageh.html" target="_blank">stb_image.h – Texture Loader</a></li>
    <li>OpenGL & StackOverflow for reference and debugging</li>
  </ul>
</body>
</html>
