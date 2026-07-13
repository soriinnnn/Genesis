# Genesis

Motor de videojocs 3D escrit en C++20 com a PFG d'Enginyeria Informàtica a la Universitat de Girona.

L'objectiu del projecte és reproduir, a escala reduïda, l'arquitectura interna d'un motor de videojocs modern, implementant des de zero els següents subsistemes: renderitzat, física, entitats, recursos, entrada d'usuari, interfície d'usuari i scripts.

## Característiques

* Renderitzat 3D en temps real mitjançant **Direct3D 11**
* Arquitectura basada en entitats i components
* Sistema de recursos amb càrrega i reutilització automàtica
* Motor de física integrat amb **Jolt Physics**
* Sistema de scripts per desacoblar la lògica del joc
* Interfície d'usuari integrada
* Gestió de materials, textures i shaders
* Suport per a:
  * Skybox
  * Il·luminació
  * Postprocessat
  * Anti-aliasing
  * Filtratge de textures
* Demostració tècnica jugable desenvolupada amb el motor

## Entorn de desenvolupament

### Tecnologies utilitzades

* C++20
* Direct3D 11
* Visual Studio 2026

### Llibreries de tercers

* Jolt Physics
* Assimp
* DirectXTex
* DirectX Tool Kit
* nlohmann/json
* xxHash

## Compilació

### Requisits previs

* Windows
* Visual Studio 2026

### Clonar el repositori

```bash
git clone https://github.com/soriinnnn/Genesis.git
```

### Compilar

Obre la solució de Visual Studio (`.sln`) i compila el projecte en mode **Debug** o **Release**.
