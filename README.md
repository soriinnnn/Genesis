# Genesis

**Genesis** és un motor de videojocs 3D escrit en **C++20** com a projecte final de grau d'Enginyeria Informàtica a la Universitat de Girona.

L'objectiu del projecte és reproduir, a escala reduïda, l'arquitectura interna d'un motor de videojocs modern, implementant des de zero els seus subsistemes principals: renderitzat, gestió d'entitats, recursos, física, entrada d'usuari, interfície gràfica i scripts.

---

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

---

## Tecnologies utilitzades

* **C++20**
* **Direct3D 11**
* **Visual Studio**
* **Git**

### Llibreries de tercers

* Jolt Physics
* Assimp
* DirectXTex
* DirectX Tool Kit
* nlohmann/json
* xxHash

---

## Compilació

### Requisits

* Windows
* Visual Studio 2026

### Clonar el repositori

```bash
git clone https://github.com/soriinnnn/Genesis.git
```

### Compilar

Obre la solució de Visual Studio (`.sln`) i compila el projecte en mode **Debug** o **Release**.
