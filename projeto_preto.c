 //login
    while (1) {
        printf("\nLOGIN UAIFOOD RESTAURANTE\n");
        printf("Digite o codigo do restaurante (ou um numero negativo para sair):\n");
        scanf("%d", &codigoRestaurante);

        if (codigoRestaurante < 0) {
        break;
        }

        int restauranteEncontrado = 0;
        for (i = 0; i < 3; i++) {
        if (restaurantes[i].codigo == codigoRestaurante) {
        restauranteEncontrado = 1;
        break;
            }
        }

        if (!restauranteEncontrado) {
        printf("Restaurante nao encontrado. Tente novamente.\n");
        continue; 
        }
