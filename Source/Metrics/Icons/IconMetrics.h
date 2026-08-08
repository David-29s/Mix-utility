#pragma once

namespace IconMetrics
{
    //==========================================================
    // General
    //==========================================================

    // Grosor de todas las líneas.
    // Afecta arcos, círculos, líneas y contornos.
    inline constexpr float thickness = 2.3f;

    // Tamaño de la fuente del texto inferior.
    // Ejemplo: BYPASS, MONO, PHASE...
    inline constexpr float labelFont = 13.0f;

    // Distancia vertical entre el icono y el texto.
    // Valores mayores bajan el texto.
    // Valores menores lo acercan al icono.
    inline constexpr float labelOffset = 18.0f;

    // Tamaño base de todos los iconos.
    // Todos los dibujos deben calcularse a partir de esta medida.
    // Si cambia este valor, todos los iconos crecerán o disminuirán
    // manteniendo sus proporciones.
    inline constexpr float iconSize = 17.0f;

    //Medida circulo de seleccion gris
    inline constexpr float glowRadius = 20.0f;
}