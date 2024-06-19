#include <gtk/gtk.h>
#include "sistema.h"

#define TAMANIO_PAGINA 15

Sistema sistema;
GtkWidget *estudiantes_table;
GtkWidget *materias_table;
static int pagina_actual_estudiantes = 1;
static int pagina_actual_materias = 1;
static int total_paginas_estudiantes = 0;
static int total_paginas_materias = 0;
GtkWidget *estudiantes_pagina_label;
GtkWidget *materias_pagina_label;

void update_tables(void);
void fill_estudiantes_table(GtkWidget *table, LinkedList *estudiantes, int pagina_actual);
void fill_materias_table(GtkWidget *table, LinkedList *materias, int pagina_actual);
static void on_modify_estudiante_button_clicked(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
static void on_delete_estudiante_button_clicked(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
static void on_modify_materia_button_clicked(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
static void on_delete_materia_button_clicked(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
void modificar_estudiante_dialog(int id);
void modificar_materia_dialog(int id);

void calcular_total_paginas(void) {
    total_paginas_estudiantes = (size(sistema.estudiantes) + TAMANIO_PAGINA - 1) / TAMANIO_PAGINA;
    total_paginas_materias = (size(sistema.materias) + TAMANIO_PAGINA - 1) / TAMANIO_PAGINA;
}

static void anterior_estudiantes_pagina(GtkWidget *widget __attribute__((unused)), gpointer data __attribute__((unused))) {
    if (pagina_actual_estudiantes > 1) {
        pagina_actual_estudiantes--;
        update_tables();
    }
}

static void siguiente_estudiantes_pagina(GtkWidget *widget __attribute__((unused)), gpointer data __attribute__((unused))) {
    if (pagina_actual_estudiantes < total_paginas_estudiantes) {
        pagina_actual_estudiantes++;
        update_tables();
    }
}

static void anterior_materias_pagina(GtkWidget *widget __attribute__((unused)), gpointer data __attribute__((unused))) {
    if (pagina_actual_materias > 1) {
        pagina_actual_materias--;
        update_tables();
    }
}

static void siguiente_materias_pagina(GtkWidget *widget __attribute__((unused)), gpointer data __attribute__((unused))) {
    if (pagina_actual_materias < total_paginas_materias) {
        pagina_actual_materias++;
        update_tables();
    }
}

void modificar_estudiante_dialog(int id) {
    GtkWidget *dialog, *content_area, *entry_nombre, *entry_edad, *grid;

    Estudiante *est = buscarEstudiantePorId(sistema.estudiantes,id);

    dialog = gtk_dialog_new_with_buttons("Modificar Estudiante", GTK_WINDOW(gtk_widget_get_toplevel(estudiantes_table)), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_OK", GTK_RESPONSE_OK, 
                                         "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nombre:"), 0, 0, 1, 1);
    entry_nombre = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_nombre), est->nombre);
    gtk_grid_attach(GTK_GRID(grid), entry_nombre, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Edad:"), 0, 1, 1, 1);
    entry_edad = gtk_entry_new();
    char edad_str[10];
    snprintf(edad_str, 10, "%d", est->edad);
    gtk_entry_set_text(GTK_ENTRY(entry_edad), edad_str);
    gtk_grid_attach(GTK_GRID(grid), entry_edad, 1, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *nombre = gtk_entry_get_text(GTK_ENTRY(entry_nombre));
        int edad = atoi(gtk_entry_get_text(GTK_ENTRY(entry_edad)));
        if (edad <= 0) {
            edad = est->edad;
        }
        modificarEstudiante(sistema.estudiantes, id, nombre, edad);
        update_tables();
    }
    gtk_widget_destroy(dialog);
}

void modificar_materia_dialog(int id) {
    GtkWidget *dialog, *content_area, *entry_nombre, *grid;

    Materia *mat = buscarMateriaPorId(sistema.materias, id);

    dialog = gtk_dialog_new_with_buttons("Modificar Materia", GTK_WINDOW(gtk_widget_get_toplevel(materias_table)), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_OK", GTK_RESPONSE_OK, 
                                         "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nombre:"), 0, 0, 1, 1);
    entry_nombre = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_nombre), mat->nombre);
    gtk_grid_attach(GTK_GRID(grid), entry_nombre, 1, 0, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *nombre = gtk_entry_get_text(GTK_ENTRY(entry_nombre));
        modificarMateria(sistema.materias, id, nombre);
        update_tables();
    }
    gtk_widget_destroy(dialog);
}

static void on_modify_estudiante_button_clicked(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data __attribute__((unused))) {
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(model, &iter, path);

    int id;
    gtk_tree_model_get(model, &iter, 0, &id, -1);

    if (gtk_tree_view_column_get_title(column) && g_strcmp0(gtk_tree_view_column_get_title(column), "Modificar") == 0) {
        modificar_estudiante_dialog(id);
    }
}

static void on_delete_estudiante_button_clicked(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data __attribute__((unused))) {
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(model, &iter, path);

    int id;
    gtk_tree_model_get(model, &iter, 0, &id, -1);

    if (gtk_tree_view_column_get_title(column) && g_strcmp0(gtk_tree_view_column_get_title(column), "Eliminar") == 0) {
        eliminarEstudiante(sistema.estudiantes, id);
        update_tables();
    }
}

static void on_modify_materia_button_clicked(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data __attribute__((unused))) {
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(model, &iter, path);

    int id;
    gtk_tree_model_get(model, &iter, 0, &id, -1);

    if (gtk_tree_view_column_get_title(column) && g_strcmp0(gtk_tree_view_column_get_title(column), "Modificar") == 0) {
        modificar_materia_dialog(id);
    }
}

static void on_delete_materia_button_clicked(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data __attribute__((unused))) {
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(model, &iter, path);

    int id;
    gtk_tree_model_get(model, &iter, 0, &id, -1);

    if (gtk_tree_view_column_get_title(column) && g_strcmp0(gtk_tree_view_column_get_title(column), "Eliminar") == 0) {
        eliminarMateria(sistema.materias, id);
        update_tables();
    }
}

void fill_tree_view(GtkListStore *store, LinkedList *notas) {
    GtkTreeIter iter;
    Node *node = notas->head;
    while (node != NULL) {
        NotasMateria *nota = (NotasMateria *)node->value;
        Materia *materia =  buscarMateriaPorId(sistema.materias,nota->id_materia);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, nota->id_materia, 1, materia->nombre, 2, nota->nota, -1);
        node = node->next;
    }
}

void add_columns_to_tree_view(GtkWidget *tree_view) {
    GtkCellRenderer *renderer;

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view), -1, "ID Materia", renderer, "text", 0, NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view), -1, "Nombre Materia", renderer, "text", 1, NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view), -1, "Nota", renderer, "text", 2, NULL);
}

void materias_estudiante_dialog(int id_estudiante) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sus Materias");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);
    gtk_widget_set_name(notebook, "notebook");

    GtkWidget *tree_view_aprobadas = gtk_tree_view_new();
    GtkWidget *tree_view_inscriptas = gtk_tree_view_new();
    GtkWidget *tree_view_desaprobadas = gtk_tree_view_new();

    GtkStyleContext *context = gtk_widget_get_style_context(tree_view_aprobadas);
    gtk_style_context_add_class(context, "font");
    GtkStyleContext *context1 = gtk_widget_get_style_context(tree_view_inscriptas);
    gtk_style_context_add_class(context1, "font");
    GtkStyleContext *context2 = gtk_widget_get_style_context(tree_view_desaprobadas);
    gtk_style_context_add_class(context2, "font");

    GtkListStore *store_aprobadas = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
    GtkListStore *store_inscriptas = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
    GtkListStore *store_desaprobadas = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);

    LinkedList *notas_aprobadas = listarNotasAprobadas(sistema.notasMateria, id_estudiante);
    LinkedList *notas_inscriptas = listarNotasInscriptas(sistema.notasMateria, id_estudiante);
    LinkedList *notas_desaprobadas = listarNotasDesaprobadas(sistema.notasMateria, id_estudiante);

    fill_tree_view(store_aprobadas, notas_aprobadas);
    fill_tree_view(store_inscriptas, notas_inscriptas);
    fill_tree_view(store_desaprobadas, notas_desaprobadas);

    gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view_aprobadas), GTK_TREE_MODEL(store_aprobadas));
    gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view_inscriptas), GTK_TREE_MODEL(store_inscriptas));
    gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view_desaprobadas), GTK_TREE_MODEL(store_desaprobadas));

    GtkTreeSelection *selection_aprobadas = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view_aprobadas));
    GtkTreeSelection *selection_inscriptas = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view_inscriptas));
    GtkTreeSelection *selection_desaprobadas = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view_desaprobadas));

    gtk_tree_selection_set_mode(selection_aprobadas, GTK_SELECTION_NONE);
    gtk_tree_selection_set_mode(selection_inscriptas, GTK_SELECTION_NONE);
    gtk_tree_selection_set_mode(selection_desaprobadas, GTK_SELECTION_NONE);

    add_columns_to_tree_view(tree_view_aprobadas);
    add_columns_to_tree_view(tree_view_inscriptas);
    add_columns_to_tree_view(tree_view_desaprobadas);

    GtkWidget *tab_label_aprobadas = gtk_label_new("Aprobadas");
    GtkWidget *tab_label_inscriptas = gtk_label_new("Inscriptas");
    GtkWidget *tab_label_desaprobadas = gtk_label_new("Desaprobadas");

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tree_view_aprobadas, tab_label_aprobadas);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tree_view_inscriptas, tab_label_inscriptas);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tree_view_desaprobadas, tab_label_desaprobadas);

    gtk_widget_show_all(window);
}

static void on_materias_estudiante_button_clicked(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data __attribute__((unused))) {
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(model, &iter, path);

    int id_estudiante;
    gtk_tree_model_get(model, &iter, 0, &id_estudiante, -1);

    if (gtk_tree_view_column_get_title(column) && g_strcmp0(gtk_tree_view_column_get_title(column), "Sus Materias") == 0) {
        materias_estudiante_dialog(id_estudiante);
    }
}


void promedio_estudiante_dialog(int id_estudiante) {
    int promedio = promedioDelEstudiante(sistema.notasMateria, id_estudiante);
    Estudiante *est = buscarEstudiantePorId(sistema.estudiantes, id_estudiante);

    gchar *mensaje = g_strdup_printf("El promedio del estudiante %s es %d", est->nombre, promedio);
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", mensaje);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_style_context_add_class(context, "font");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(mensaje);
}

static void on_promedio_estudiante_button_clicked(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data __attribute__((unused))) {
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(model, &iter, path);

    int id_estudiante;
    gtk_tree_model_get(model, &iter, 0, &id_estudiante, -1);

    if (gtk_tree_view_column_get_title(column) && g_strcmp0(gtk_tree_view_column_get_title(column), "Promedio") == 0) {
        promedio_estudiante_dialog(id_estudiante);
    }
}


void fill_estudiantes_table(GtkWidget *table, LinkedList *estudiantes, int pagina_actual) {
    GtkListStore *store;
    GtkTreeIter iter;

    store = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
    GtkStyleContext *context = gtk_widget_get_style_context(table);
    gtk_style_context_add_class(context, "font");

    int inicio = (pagina_actual - 1) * TAMANIO_PAGINA;
    int fin = inicio + TAMANIO_PAGINA;

    Node *node = estudiantes->head;
    int contador = 0;
    while (node != NULL && contador < fin) {
        if (contador >= inicio) {
            Estudiante *est = (Estudiante *)node->value;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, est->id, 1, est->nombre, 2, est->edad, -1);
        }
        contador++;
        node = node->next;
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(table), GTK_TREE_MODEL(store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(table), TRUE);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(table));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);

    if (gtk_tree_view_get_n_columns(GTK_TREE_VIEW(table)) == 0) {
        GtkCellRenderer *renderer;

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(table), -1, "ID", renderer, "text", 0, NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(table), -1, "Nombre", renderer, "text", 1, NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(table), -1, "Edad", renderer, "text", 2, NULL);

        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "text", "Promedio", "foreground", "#000000", "background", "#ffc107", "foreground-set", TRUE, "background-set", TRUE, "xalign", 0.5, NULL);
        GtkTreeViewColumn *col_promedio = gtk_tree_view_column_new_with_attributes("Promedio", renderer, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(table), col_promedio);
        g_signal_connect(table, "row-activated", G_CALLBACK(on_promedio_estudiante_button_clicked), NULL);

        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "text", "Sus Materias", "foreground", "#ffffff", "background", "#17a2b8", "foreground-set", TRUE, "background-set", TRUE, "xalign", 0.5, NULL);
        GtkTreeViewColumn *col_materias = gtk_tree_view_column_new_with_attributes("Sus Materias", renderer, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(table), col_materias);
        g_signal_connect(table, "row-activated", G_CALLBACK(on_materias_estudiante_button_clicked), NULL);

        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "text", "Modificar", "foreground", "#ffffff", "background", "#007bff", "foreground-set", TRUE, "background-set", TRUE, "xalign", 0.5, NULL);
        GtkTreeViewColumn *col_modificar = gtk_tree_view_column_new_with_attributes("Modificar", renderer, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(table), col_modificar);
        g_signal_connect(table, "row-activated", G_CALLBACK(on_modify_estudiante_button_clicked), NULL);

        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "text", "Eliminar", "foreground", "#ffffff", "background", "#dc3545", "foreground-set", TRUE, "background-set", TRUE, "xalign", 0.5, NULL);
        GtkTreeViewColumn *col_eliminar = gtk_tree_view_column_new_with_attributes("Eliminar", renderer, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(table), col_eliminar);
        g_signal_connect(table, "row-activated", G_CALLBACK(on_delete_estudiante_button_clicked), NULL);

        GtkCellRenderer *renderer_blank = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *col_blank = gtk_tree_view_column_new();
        gtk_tree_view_column_pack_start(col_blank, renderer_blank, TRUE);
        gtk_tree_view_column_set_fixed_width(col_blank, 20);
        gtk_tree_view_append_column(GTK_TREE_VIEW(table), col_blank);   

    }
}

void fill_materias_table(GtkWidget *table, LinkedList *materias, int pagina_actual) {
    GtkListStore *store;
    GtkTreeIter iter;

    store = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_STRING);
    GtkStyleContext *context = gtk_widget_get_style_context(table);
    gtk_style_context_add_class(context, "font");

    int inicio = (pagina_actual - 1) * TAMANIO_PAGINA;
    int fin = inicio + TAMANIO_PAGINA;

    Node *node = materias->head;
    int contador = 0;
    while (node != NULL && contador < fin) {
        if (contador >= inicio) {
            Materia *mat = (Materia *)node->value;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, mat->id, 1, mat->nombre, -1);
        }
        contador++;
        node = node->next;
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(table), GTK_TREE_MODEL(store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(table), TRUE);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(table));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);

    if (gtk_tree_view_get_n_columns(GTK_TREE_VIEW(table)) == 0) {
        GtkCellRenderer *renderer;

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(table), -1, "ID", renderer, "text", 0, NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(table), -1, "Nombre", renderer, "text", 1, NULL);

        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "text", "Modificar", "foreground", "#ffffff", "background", "#007bff", "foreground-set", TRUE, "background-set", TRUE, "xalign", 0.5, NULL);
        GtkTreeViewColumn *col_modificar = gtk_tree_view_column_new_with_attributes("Modificar", renderer, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(table), col_modificar);
        g_signal_connect(table, "row-activated", G_CALLBACK(on_modify_materia_button_clicked), NULL);

        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "text", "Eliminar", "foreground", "#ffffff", "background", "#dc3545", "foreground-set", TRUE, "background-set", TRUE, "xalign", 0.5, NULL);
        GtkTreeViewColumn *col_eliminar = gtk_tree_view_column_new_with_attributes("Eliminar", renderer, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(table), col_eliminar);
        g_signal_connect(table, "row-activated", G_CALLBACK(on_delete_materia_button_clicked), NULL);

        GtkCellRenderer *renderer_blank = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *col_blank = gtk_tree_view_column_new();
        gtk_tree_view_column_pack_start(col_blank, renderer_blank, TRUE);
        gtk_tree_view_column_set_fixed_width(col_blank, 20);
        gtk_tree_view_append_column(GTK_TREE_VIEW(table), col_blank); 
    }
}

void update_tables(void) {
    calcular_total_paginas();
    
    fill_estudiantes_table(estudiantes_table, sistema.estudiantes, pagina_actual_estudiantes);
    fill_materias_table(materias_table, sistema.materias, pagina_actual_materias);

    char label_text[50];

    snprintf(label_text, sizeof(label_text), "Página %d de %d", pagina_actual_estudiantes, total_paginas_estudiantes);
    gtk_label_set_text(GTK_LABEL(estudiantes_pagina_label), label_text);

    snprintf(label_text, sizeof(label_text), "Página %d de %d", pagina_actual_materias, total_paginas_materias);
    gtk_label_set_text(GTK_LABEL(materias_pagina_label), label_text);

    guardarEstudiantesCSV(&sistema,"estudiantes.csv");
    guardarMateriasCSV(&sistema,"materias.csv");
    guardarNotasCSV(&sistema,"notasMateria.csv");
    
}

static void agregar_estudiante_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    GtkWidget *dialog, *content_area, *entry_nombre, *entry_edad, *grid;
    
    dialog = gtk_dialog_new_with_buttons("Agregar Estudiante", GTK_WINDOW(window), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_OK", GTK_RESPONSE_OK, 
                                         "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nombre:"), 0, 0, 1, 1);
    entry_nombre = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_nombre, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Edad:"), 0, 1, 1, 1);
    entry_edad = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_edad, 1, 1, 1, 1);
    
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *nombre = gtk_entry_get_text(GTK_ENTRY(entry_nombre));
        int edad = atoi(gtk_entry_get_text(GTK_ENTRY(entry_edad)));
        agregarEstudiante(sistema.estudiantes, nombre, edad);
        update_tables();
    }
    gtk_widget_destroy(dialog);
}

static void agregar_materia_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    GtkWidget *dialog, *content_area, *entry_nombre, *grid;
    
    dialog = gtk_dialog_new_with_buttons("Agregar Materia", GTK_WINDOW(window), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_OK", GTK_RESPONSE_OK, 
                                         "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nombre:"), 0, 0, 1, 1);
    entry_nombre = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_nombre, 1, 0, 1, 1);
    
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *nombre = gtk_entry_get_text(GTK_ENTRY(entry_nombre));
        agregarMateria(sistema.materias, nombre);
        update_tables();
    }
    gtk_widget_destroy(dialog);
}

static void buscar_estudiante_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    GtkWidget *dialog, *content_area, *entry_nombre, *grid;
    
    dialog = gtk_dialog_new_with_buttons("Buscar Estudiante", GTK_WINDOW(window), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_Buscar", GTK_RESPONSE_OK, 
                                         "_Cancelar", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nombre:"), 0, 0, 1, 1);
    entry_nombre = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_nombre, 1, 0, 1, 1);
    
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *nombre = gtk_entry_get_text(GTK_ENTRY(entry_nombre));
        Estudiante *est = buscarEstudiantePorNombre(sistema.estudiantes, nombre);
        if (est != NULL) {
            GtkWidget *result_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                              GTK_DIALOG_MODAL,
                                                              GTK_MESSAGE_INFO,
                                                              GTK_BUTTONS_OK,
                                                              "Estudiante encontrado:\n\nNombre: %s\nEdad: %d",
                                                              est->nombre,
                                                              est->edad);
            GtkStyleContext *context = gtk_widget_get_style_context(result_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(result_dialog));
            gtk_widget_destroy(result_dialog);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                             GTK_DIALOG_MODAL,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_OK,
                                                             "Estudiante no encontrado");
            GtkStyleContext *context = gtk_widget_get_style_context(error_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }
    gtk_widget_destroy(dialog);
}

static void buscar_estudiante_por_id_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    GtkWidget *dialog, *content_area, *entry_id, *grid;
    
    dialog = gtk_dialog_new_with_buttons("Buscar Estudiante por ID", GTK_WINDOW(window), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_Buscar", GTK_RESPONSE_OK, 
                                         "_Cancelar", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID:"), 0, 0, 1, 1);
    entry_id = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_id), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);
    
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
        int id = atoi(id_text);
        Estudiante *est = buscarEstudiantePorId(sistema.estudiantes, id);
        if (est != NULL) {
            GtkWidget *result_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                              GTK_DIALOG_MODAL,
                                                              GTK_MESSAGE_INFO,
                                                              GTK_BUTTONS_OK,
                                                              "Estudiante encontrado:\n\nID: %d\nNombre: %s\nEdad: %d",
                                                              est->id,
                                                              est->nombre,
                                                              est->edad);
            GtkStyleContext *context = gtk_widget_get_style_context(result_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(result_dialog));
            gtk_widget_destroy(result_dialog);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                             GTK_DIALOG_MODAL,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_OK,
                                                             "Estudiante no encontrado");
            GtkStyleContext *context = gtk_widget_get_style_context(error_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }
    gtk_widget_destroy(dialog);
}

GtkWidget* crear_tree_view_estudiantes(LinkedList *lista) {
    GtkWidget *tree_view;
    GtkListStore *list_store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    list_store = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);

    Node *current = lista->head;
    while (current != NULL) {
        Estudiante *est = (Estudiante*)current->value;
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter,
                           0, est->id,
                           1, est->nombre,
                           2, est->edad,
                           -1);
        current = current->next;
    }

    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    g_object_unref(list_store);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);
    GtkStyleContext *context = gtk_widget_get_style_context(tree_view);
    gtk_style_context_add_class(context, "font");

    renderer = gtk_cell_renderer_text_new();

    column = gtk_tree_view_column_new_with_attributes("ID",
                                                      renderer,
                                                      "text", 0,
                                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("Nombre",
                                                      renderer,
                                                      "text", 1,
                                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("Edad",
                                                      renderer,
                                                      "text", 2,
                                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "text", "Promedio", "foreground", "#000000", "background", "#ffc107", "foreground-set", TRUE, "background-set", TRUE, "xalign", 0.5, NULL);
    GtkTreeViewColumn *col_promedio = gtk_tree_view_column_new_with_attributes("Promedio", renderer, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col_promedio);
    g_signal_connect(tree_view, "row-activated", G_CALLBACK(on_promedio_estudiante_button_clicked), NULL);
    
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "text", "Sus Materias", "foreground", "#ffffff", "background", "#17a2b8", "foreground-set", TRUE, "background-set", TRUE, "xalign", 0.5, NULL);
    GtkTreeViewColumn *col_materias = gtk_tree_view_column_new_with_attributes("Sus Materias", renderer, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col_materias);
    g_signal_connect(tree_view, "row-activated", G_CALLBACK(on_materias_estudiante_button_clicked), NULL);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "text", "Modificar", "foreground", "#ffffff", "background", "#007bff", "foreground-set", TRUE, "background-set", TRUE, "xalign", 0.5, NULL);
    GtkTreeViewColumn *col_modificar = gtk_tree_view_column_new_with_attributes("Modificar", renderer, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col_modificar);
    g_signal_connect(tree_view, "row-activated", G_CALLBACK(on_modify_estudiante_button_clicked), NULL);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "text", "Eliminar", "foreground", "#ffffff", "background", "#dc3545", "foreground-set", TRUE, "background-set", TRUE, "xalign", 0.5, NULL);
    GtkTreeViewColumn *col_eliminar = gtk_tree_view_column_new_with_attributes("Eliminar", renderer, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col_eliminar);
    g_signal_connect(tree_view, "row-activated", G_CALLBACK(on_delete_estudiante_button_clicked), NULL);

    GtkCellRenderer *renderer_blank = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *col_blank = gtk_tree_view_column_new();
    gtk_tree_view_column_pack_start(col_blank, renderer_blank, TRUE);
    gtk_tree_view_column_set_fixed_width(col_blank, 20);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col_blank);

    return tree_view;
}

static void buscar_estudiantes_por_rango_edad_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    GtkWidget *dialog, *content_area, *entry_edad_min, *entry_edad_max, *grid;
    
    dialog = gtk_dialog_new_with_buttons("Buscar Estudiantes por Rango de Edad", GTK_WINDOW(window), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_Buscar", GTK_RESPONSE_OK, 
                                         "_Cancelar", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Edad Mínima:"), 0, 0, 1, 1);
    entry_edad_min = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_edad_min), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_edad_min, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Edad Máxima:"), 0, 1, 1, 1);
    entry_edad_max = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_edad_max), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_edad_max, 1, 1, 1, 1);
    
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *edad_min_text = gtk_entry_get_text(GTK_ENTRY(entry_edad_min));
        const char *edad_max_text = gtk_entry_get_text(GTK_ENTRY(entry_edad_max));
        int edad_min = atoi(edad_min_text);
        int edad_max = atoi(edad_max_text);
        
        LinkedList *resultados = buscarEstudiantesPorRangoDeEdad(sistema.estudiantes, edad_min, edad_max);
        
        GtkWidget *result_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(result_window), "Resultados de la Búsqueda");
        gtk_window_set_default_size(GTK_WINDOW(result_window), 200, 500);
        gtk_window_set_position (GTK_WINDOW(result_window),GTK_WIN_POS_CENTER);

        GtkWidget *tree_view = crear_tree_view_estudiantes(resultados);
        gtk_container_add(GTK_CONTAINER(result_window), tree_view);

        gtk_widget_show_all(result_window);
    }
    gtk_widget_destroy(dialog);
}

static void mostrar_estadisticas_estudiantes_callback(GtkWidget *widget __attribute__((unused)), gpointer data __attribute__((unused))) {
    int promedio_edad = mostrarEstadisticasEstudiantes(sistema.estudiantes);
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Promedio de edad de los estudiantes: %d", promedio_edad);

    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_INFO,
                                                GTK_BUTTONS_OK,
                                                "%s", buffer);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void buscar_materia_por_nombre_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    GtkWidget *dialog, *content_area, *entry_nombre, *grid;
    
    dialog = gtk_dialog_new_with_buttons("Buscar Materia por Nombre", GTK_WINDOW(window), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_Buscar", GTK_RESPONSE_OK, 
                                         "_Cancelar", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nombre:"), 0, 0, 1, 1);
    entry_nombre = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_nombre, 1, 0, 1, 1);
    
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *nombre_text = gtk_entry_get_text(GTK_ENTRY(entry_nombre));
        Materia *mat = buscarMateriaPorNombre(sistema.materias, nombre_text);
        if (mat != NULL) {
            GtkWidget *result_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                              GTK_DIALOG_MODAL,
                                                              GTK_MESSAGE_INFO,
                                                              GTK_BUTTONS_OK,
                                                              "Materia encontrada:\n\nNombre: %s",
                                                              mat->nombre);
            GtkStyleContext *context = gtk_widget_get_style_context(result_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(result_dialog));
            gtk_widget_destroy(result_dialog);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                             GTK_DIALOG_MODAL,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_OK,
                                                             "Materia no encontrada");
            GtkStyleContext *context = gtk_widget_get_style_context(error_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }
    gtk_widget_destroy(dialog);
}

static void buscar_materia_por_id_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    GtkWidget *dialog, *content_area, *entry_id, *grid;
    
    dialog = gtk_dialog_new_with_buttons("Buscar Materia por ID", GTK_WINDOW(window), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_Buscar", GTK_RESPONSE_OK, 
                                         "_Cancelar", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID:"), 0, 0, 1, 1);
    entry_id = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_id), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);
    
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
        int id = atoi(id_text);
        Materia *mat = buscarMateriaPorId(sistema.materias, id);
        if (mat != NULL) {
            GtkWidget *result_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                              GTK_DIALOG_MODAL,
                                                              GTK_MESSAGE_INFO,
                                                              GTK_BUTTONS_OK,
                                                              "Materia encontrada:\n\nNombre: %s",
                                                              mat->nombre);
            GtkStyleContext *context = gtk_widget_get_style_context(result_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(result_dialog));
            gtk_widget_destroy(result_dialog);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                             GTK_DIALOG_MODAL,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_OK,
                                                             "Materia no encontrada");
            GtkStyleContext *context = gtk_widget_get_style_context(error_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }
    gtk_widget_destroy(dialog);
}

static void mostrar_estadisticas_materias_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    int cantidad_materias = mostrarEstadisticasMaterias(sistema.materias);
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Cantidad de materias: %d", cantidad_materias);

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_INFO,
                                                GTK_BUTTONS_OK,
                                                "%s", buffer);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void generar_pruebas_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    GtkWidget *dialog, *content_area, *entry_cantidad_est, *entry_cantidad_mat, *grid;
    
    dialog = gtk_dialog_new_with_buttons("Generar informacion de prueba", GTK_WINDOW(window), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_Ok", GTK_RESPONSE_OK, 
                                         "_Cancelar", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Cantidad de estudiantes:"), 0, 0, 1, 1);
    entry_cantidad_est = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_cantidad_est), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_cantidad_est, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Cantidad de materias:"), 0, 1, 1, 1);
    entry_cantidad_mat = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_cantidad_mat), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_cantidad_mat, 1, 1, 1, 1);
    
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *cantidad_est_text = gtk_entry_get_text(GTK_ENTRY(entry_cantidad_est));
        const char *cantidad_mat_text = gtk_entry_get_text(GTK_ENTRY(entry_cantidad_mat));
        int cantidad_est = atoi(cantidad_est_text);
        int cantidad_mat = atoi(cantidad_mat_text);
        generarEstudiantesDePrueba(&sistema, cantidad_est);
        generarMateriasDePrueba(&sistema, cantidad_mat);
        update_tables();
    }
    gtk_widget_destroy(dialog);
}

static void inscribir_en_materia_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    GtkWidget *dialog, *content_area, *entry_id_estudiante, *entry_id_materia, *grid;
    
    dialog = gtk_dialog_new_with_buttons("Inscribir en Materia", GTK_WINDOW(window), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_Inscribir", GTK_RESPONSE_OK, 
                                         "_Cancelar", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID Estudiante:"), 0, 0, 1, 1);
    entry_id_estudiante = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_id_estudiante), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_id_estudiante, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID Materia:"), 0, 1, 1, 1);
    entry_id_materia = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_id_materia), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_id_materia, 1, 1, 1, 1);
    
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *id_estudiante_text = gtk_entry_get_text(GTK_ENTRY(entry_id_estudiante));
        int id_estudiante = atoi(id_estudiante_text);
        const char *id_materia_text = gtk_entry_get_text(GTK_ENTRY(entry_id_materia));
        int id_materia = atoi(id_materia_text);
        
        bool puede_anotarse = puedeAnotarse(sistema.notasMateria, id_estudiante, id_materia);
        if (puede_anotarse) {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                             GTK_DIALOG_MODAL,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_OK,
                                                             "No se puede inscribir en una materia ya aprobada o ya esta inscripto.");
            GtkStyleContext *context = gtk_widget_get_style_context(error_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        } else {
            inscribirEnMateria(sistema.notasMateria, id_estudiante, id_materia);
            GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                               GTK_DIALOG_MODAL,
                                                               GTK_MESSAGE_INFO,
                                                               GTK_BUTTONS_OK,
                                                               "Inscripción realizada con éxito.");
            GtkStyleContext *context = gtk_widget_get_style_context(success_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(success_dialog));
            gtk_widget_destroy(success_dialog);
            update_tables();
        }
    }
    gtk_widget_destroy(dialog);
}

static void rendir_materia_callback(GtkWidget *widget __attribute__((unused)), gpointer window) {
    GtkWidget *dialog, *content_area, *entry_id_estudiante, *entry_id_materia, *entry_nota, *grid;
    
    dialog = gtk_dialog_new_with_buttons("Rendir Materia", GTK_WINDOW(window), 
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
                                         "_Rendir", GTK_RESPONSE_OK, 
                                         "_Cancelar", GTK_RESPONSE_CANCEL, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "font");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID Estudiante:"), 0, 0, 1, 1);
    entry_id_estudiante = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_id_estudiante), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_id_estudiante, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID Materia:"), 0, 1, 1, 1);
    entry_id_materia = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_id_materia), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_id_materia, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nota:"), 0, 2, 1, 1);
    entry_nota = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry_nota), GTK_INPUT_PURPOSE_DIGITS);
    gtk_grid_attach(GTK_GRID(grid), entry_nota, 1, 2, 1, 1);
    
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *id_estudiante_text = gtk_entry_get_text(GTK_ENTRY(entry_id_estudiante));
        int id_estudiante = atoi(id_estudiante_text);
        const char *id_materia_text = gtk_entry_get_text(GTK_ENTRY(entry_id_materia));
        int id_materia = atoi(id_materia_text);
        const char *nota_text = gtk_entry_get_text(GTK_ENTRY(entry_nota));
        int nota = atoi(nota_text);

        NotasMateria *nota_inscripcion = buscarNotaInscripcion(sistema.notasMateria, id_estudiante, id_materia);
        if (nota_inscripcion != NULL) {
            rendirMateria(sistema.notasMateria, nota, id_estudiante, id_materia);
            GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                               GTK_DIALOG_MODAL,
                                                               GTK_MESSAGE_INFO,
                                                               GTK_BUTTONS_OK,
                                                               "Materia rendida con éxito.");
            GtkStyleContext *context = gtk_widget_get_style_context(success_dialog);
            gtk_style_context_add_class(context, "font");
            gtk_dialog_run(GTK_DIALOG(success_dialog));
            gtk_widget_destroy(success_dialog);
            update_tables();
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                             GTK_DIALOG_MODAL,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_OK,
                                                             "No se puede rendir la materia porque ya está aprobada o nunca se inscribió.");
            GtkStyleContext *context = gtk_widget_get_style_context(error_dialog);
            gtk_style_context_add_class(context, "font"); 
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }
    gtk_widget_destroy(dialog);
}

static void toggle_revealer(GtkWidget *widget __attribute__((unused)), gpointer data) {
    GtkWidget *revealer = (GtkWidget *)data;
    gtk_revealer_set_reveal_child(GTK_REVEALER(revealer), !gtk_revealer_get_reveal_child(GTK_REVEALER(revealer)));
}

void load_css(void) {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    const gchar *css_file = "css/styles.css";
    GError *error = NULL;
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(css_file), &error);

    if (error != NULL) {
        g_warning("Error loading CSS file: %s", error->message);
        g_clear_error(&error);
    }

    g_object_unref(provider);
}

static GtkWidget *create_sidebar(void) {
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(sidebar, "sidebar");

    GtkWidget *button_estudiantes = gtk_button_new_with_label("Estudiantes");
    gtk_box_pack_start(GTK_BOX(sidebar), button_estudiantes, FALSE, FALSE, 0);
    GtkStyleContext *context = gtk_widget_get_style_context(button_estudiantes);
    gtk_style_context_add_class(context, "button-slide-down");

    GtkWidget *revealer_estudiantes = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(revealer_estudiantes), GTK_REVEALER_TRANSITION_TYPE_SLIDE_DOWN);
    gtk_box_pack_start(GTK_BOX(sidebar), revealer_estudiantes, FALSE, FALSE, 0);

    GtkWidget *revealer_box_estudiantes = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(revealer_estudiantes), revealer_box_estudiantes);

    GtkWidget *button_agregar_estudiante = gtk_button_new_with_label("Agregar Estudiante");
    g_signal_connect(button_agregar_estudiante, "clicked", G_CALLBACK(agregar_estudiante_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_estudiantes), button_agregar_estudiante, FALSE, FALSE, 0);
    GtkStyleContext *context3 = gtk_widget_get_style_context(button_agregar_estudiante);
    gtk_style_context_add_class(context3, "button-sec");

    GtkWidget *button_buscar_estudiante = gtk_button_new_with_label("Buscar Estudiante por nombre");
    g_signal_connect(button_buscar_estudiante, "clicked", G_CALLBACK(buscar_estudiante_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_estudiantes), button_buscar_estudiante, FALSE, FALSE, 0);
    GtkStyleContext *context4 = gtk_widget_get_style_context(button_buscar_estudiante);
    gtk_style_context_add_class(context4, "button-sec");

    GtkWidget *button_buscar_estudiante_por_id = gtk_button_new_with_label("Buscar Estudiante por ID");
    g_signal_connect(button_buscar_estudiante_por_id, "clicked", G_CALLBACK(buscar_estudiante_por_id_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_estudiantes), button_buscar_estudiante_por_id, FALSE, FALSE, 0);
    GtkStyleContext *context5 = gtk_widget_get_style_context(button_buscar_estudiante_por_id);
    gtk_style_context_add_class(context5, "button-sec");

    GtkWidget *button_buscar_estudiantes_por_rango_edad = gtk_button_new_with_label("Buscar Estudiantes por Rango de Edad");
    g_signal_connect(button_buscar_estudiantes_por_rango_edad, "clicked", G_CALLBACK(buscar_estudiantes_por_rango_edad_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_estudiantes), button_buscar_estudiantes_por_rango_edad, FALSE, FALSE, 0);
    GtkStyleContext *context6 = gtk_widget_get_style_context(button_buscar_estudiantes_por_rango_edad);
    gtk_style_context_add_class(context6, "button-sec");

    GtkWidget *button_mostrar_estadisticas = gtk_button_new_with_label("Promedio de edad de los estudiantes");
    g_signal_connect(button_mostrar_estadisticas, "clicked", G_CALLBACK(mostrar_estadisticas_estudiantes_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_estudiantes), button_mostrar_estadisticas, FALSE, FALSE, 0);
    GtkStyleContext *context7 = gtk_widget_get_style_context(button_mostrar_estadisticas);
    gtk_style_context_add_class(context7, "button-sec");

    GtkWidget *button_materias = gtk_button_new_with_label("Materias");
    gtk_box_pack_start(GTK_BOX(sidebar), button_materias, FALSE, FALSE, 0);
    GtkStyleContext *context8 = gtk_widget_get_style_context(button_materias);
    gtk_style_context_add_class(context8, "button-slide-down");

    GtkWidget *revealer_materias = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(revealer_materias), GTK_REVEALER_TRANSITION_TYPE_SLIDE_DOWN);
    gtk_box_pack_start(GTK_BOX(sidebar), revealer_materias, FALSE, FALSE, 0);

    GtkWidget *revealer_box_materias = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(revealer_materias), revealer_box_materias);

    GtkWidget *button_agregar_materia = gtk_button_new_with_label("Agregar Materia");
    g_signal_connect(button_agregar_materia, "clicked", G_CALLBACK(agregar_materia_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_materias), button_agregar_materia, FALSE, FALSE, 0);
    GtkStyleContext *context9 = gtk_widget_get_style_context(button_agregar_materia);
    gtk_style_context_add_class(context9, "button-sec");

    GtkWidget *button_buscar_materia = gtk_button_new_with_label("Buscar Materias por nombre");
    g_signal_connect(button_buscar_materia, "clicked", G_CALLBACK(buscar_materia_por_nombre_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_materias), button_buscar_materia, FALSE, FALSE, 0);
    GtkStyleContext *context10 = gtk_widget_get_style_context(button_buscar_materia);
    gtk_style_context_add_class(context10, "button-sec");

    GtkWidget *button_buscar_materia_por_id = gtk_button_new_with_label("Buscar Materias por ID");
    g_signal_connect(button_buscar_materia_por_id, "clicked", G_CALLBACK(buscar_materia_por_id_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_materias), button_buscar_materia_por_id, FALSE, FALSE, 0);
    GtkStyleContext *context11 = gtk_widget_get_style_context(button_buscar_materia_por_id);
    gtk_style_context_add_class(context11, "button-sec");

    GtkWidget *button_mostrar_estadisticas_materias = gtk_button_new_with_label("Mostrar Estadísticas Materias");
    g_signal_connect(button_mostrar_estadisticas_materias, "clicked", G_CALLBACK(mostrar_estadisticas_materias_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_materias), button_mostrar_estadisticas_materias, FALSE, FALSE, 0);
    GtkStyleContext *context12 = gtk_widget_get_style_context(button_mostrar_estadisticas_materias);
    gtk_style_context_add_class(context12, "button-sec");

    GtkWidget *button_inscribir_en_materia = gtk_button_new_with_label("Inscribir en Materia");
    g_signal_connect(button_inscribir_en_materia, "clicked", G_CALLBACK(inscribir_en_materia_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_materias), button_inscribir_en_materia, FALSE, FALSE, 0);
    GtkStyleContext *context13 = gtk_widget_get_style_context(button_inscribir_en_materia);
    gtk_style_context_add_class(context13, "button-sec");

    GtkWidget *button_rendir_materia = gtk_button_new_with_label("Rendir Materia");
    g_signal_connect(button_rendir_materia, "clicked", G_CALLBACK(rendir_materia_callback), NULL);
    gtk_box_pack_start(GTK_BOX(revealer_box_materias), button_rendir_materia, FALSE, FALSE, 0);
    GtkStyleContext *context14 = gtk_widget_get_style_context(button_rendir_materia);
    gtk_style_context_add_class(context14, "button-sec");
    
    g_signal_connect(button_estudiantes, "clicked", G_CALLBACK(toggle_revealer), revealer_estudiantes);
    g_signal_connect(button_materias, "clicked", G_CALLBACK(toggle_revealer), revealer_materias);

    GtkWidget *button_generar_pruebas = gtk_button_new_with_label("Generar informacion de prueba");
    g_signal_connect(button_generar_pruebas, "clicked", G_CALLBACK(generar_pruebas_callback), NULL);
    gtk_box_pack_start(GTK_BOX(sidebar), button_generar_pruebas, FALSE, FALSE, 0);
    GtkStyleContext *context15 = gtk_widget_get_style_context(button_generar_pruebas);
    gtk_style_context_add_class(context15, "button-slide-down");

    return sidebar;
}

static void switch_page_callback(GtkNotebook *notebook, GtkWidget *page __attribute__((unused)), guint page_num, gpointer user_data) {
    GtkWidget *estudiantes_pagination_box = (GtkWidget *)user_data;
    GtkWidget *materias_pagination_box = (GtkWidget *)g_object_get_data(G_OBJECT(notebook), "materias_pagination_box");

    if (page_num == 0) {
        gtk_widget_show(estudiantes_pagination_box);
        gtk_widget_hide(materias_pagination_box);
    } else {
        gtk_widget_hide(estudiantes_pagination_box);
        gtk_widget_show(materias_pagination_box);
    }
}


static GtkWidget *create_main_content(void) {
    GtkWidget *content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(content_area), notebook, TRUE, TRUE, 0);
    gtk_widget_set_name(notebook, "notebook");

    estudiantes_table = gtk_tree_view_new();
    GtkWidget *estudiantes_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(estudiantes_scrolled_window), estudiantes_table);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), estudiantes_scrolled_window, gtk_label_new("Estudiantes"));

    GtkWidget *estudiantes_pagination_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(content_area), estudiantes_pagination_box, FALSE, FALSE, 0);
    gtk_widget_set_name(estudiantes_pagination_box, "sidebar");

    GtkWidget *estudiantes_pagina_anterior_button = gtk_button_new_with_label("Anterior");
    g_signal_connect(estudiantes_pagina_anterior_button, "clicked", G_CALLBACK(anterior_estudiantes_pagina), NULL);
    gtk_box_pack_start(GTK_BOX(estudiantes_pagination_box), estudiantes_pagina_anterior_button, FALSE, FALSE, 0);
    GtkStyleContext *context = gtk_widget_get_style_context(estudiantes_pagina_anterior_button);
    gtk_style_context_add_class(context, "button-pagination");

    GtkWidget *estudiantes_pagina_siguiente_button = gtk_button_new_with_label("Siguiente");
    g_signal_connect(estudiantes_pagina_siguiente_button, "clicked", G_CALLBACK(siguiente_estudiantes_pagina), NULL);
    gtk_box_pack_start(GTK_BOX(estudiantes_pagination_box), estudiantes_pagina_siguiente_button, FALSE, FALSE, 0);
    GtkStyleContext *context1 = gtk_widget_get_style_context(estudiantes_pagina_siguiente_button);
    gtk_style_context_add_class(context1, "button-pagination");

    estudiantes_pagina_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(estudiantes_pagination_box), estudiantes_pagina_label, FALSE, FALSE, 0);
    GtkStyleContext *context4 = gtk_widget_get_style_context(estudiantes_pagina_label);
    gtk_style_context_add_class(context4, "paginas");


    materias_table = gtk_tree_view_new();
    GtkWidget *materias_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(materias_scrolled_window), materias_table);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), materias_scrolled_window, gtk_label_new("Materias"));

    GtkWidget *materias_pagination_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(content_area), materias_pagination_box, FALSE, FALSE, 0);
    gtk_widget_set_name(materias_pagination_box, "sidebar");

    GtkWidget *materias_pagina_anterior_button = gtk_button_new_with_label("Anterior");
    g_signal_connect(materias_pagina_anterior_button, "clicked", G_CALLBACK(anterior_materias_pagina), NULL);
    gtk_box_pack_start(GTK_BOX(materias_pagination_box), materias_pagina_anterior_button, FALSE, FALSE, 0);
    GtkStyleContext *context2 = gtk_widget_get_style_context(materias_pagina_anterior_button);
    gtk_style_context_add_class(context2, "button-pagination");

    GtkWidget *materias_pagina_siguiente_button = gtk_button_new_with_label("Siguiente");
    g_signal_connect(materias_pagina_siguiente_button, "clicked", G_CALLBACK(siguiente_materias_pagina), NULL);
    gtk_box_pack_start(GTK_BOX(materias_pagination_box), materias_pagina_siguiente_button, FALSE, FALSE, 0);
    GtkStyleContext *context3 = gtk_widget_get_style_context(materias_pagina_siguiente_button);
    gtk_style_context_add_class(context3, "button-pagination");

    materias_pagina_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(materias_pagination_box), materias_pagina_label, FALSE, FALSE, 0);
    GtkStyleContext *context5 = gtk_widget_get_style_context(materias_pagina_label);
    gtk_style_context_add_class(context5, "paginas");

    g_signal_connect(notebook, "switch-page", G_CALLBACK(switch_page_callback), estudiantes_pagination_box);

    g_object_set_data(G_OBJECT(notebook), "materias_pagination_box", materias_pagination_box);

    update_tables();

    return content_area;
}

static void activate(GtkApplication *app, gpointer user_data __attribute__((unused))) {
    
    load_css();

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Sistema de Gestión de Estudiantes y Materias");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 500);
    gtk_window_set_position (GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget *sidebar = create_sidebar();
    gtk_box_pack_start(GTK_BOX(main_box), sidebar, FALSE, FALSE, 0);

    GtkWidget *content_area = create_main_content();
    gtk_box_pack_start(GTK_BOX(main_box), content_area, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    srand(time(NULL));
    inicializarSistema(&sistema);

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}





