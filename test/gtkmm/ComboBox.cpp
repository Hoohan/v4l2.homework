#include <gtkmm/application.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>
#include <gtkmm/window.h>

#include <iostream>

class ExampleWindow : public Gtk::Window {
public:
    ExampleWindow();
    virtual ~ExampleWindow();

protected:
    void on_cell_data_extra(const Gtk::TreeModel::const_iterator& iter);

    // Signal handlers:
    void on_combo_changed();

    // Tree model columns:
    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        ModelColumns() {
            add(m_col_id);
            add(m_col_name);
            add(m_col_extra);
        }

        Gtk::TreeModelColumn<int> m_col_id;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
        Gtk::TreeModelColumn<Glib::ustring> m_col_extra;
    };

    ModelColumns m_Columns;

    // Child widgets:
    Gtk::ComboBox m_Combo;
    Gtk::CellRendererText m_cell;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    ExampleWindow window;

    // Shows the window and returns when it is closed.
    return app->run(window);
}

ExampleWindow::ExampleWindow() {
    set_title("ComboBox example");
    set_default_size(320, 240);

    // Create the Tree model:
    // m_refTreeModel = Gtk::TreeStore::create(m_Columns);
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_Combo.set_model(m_refTreeModel);

    // Fill the ComboBox's Tree Model:
    Gtk::TreeModel::Row row = *(m_refTreeModel->append());
    row[m_Columns.m_col_id] = 1;
    row[m_Columns.m_col_name] = "Billy Bob";
    row[m_Columns.m_col_extra] = "something";
    m_Combo.set_active(row);
    /*
    Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));
    childrow[m_Columns.m_col_id] = 11;
    childrow[m_Columns.m_col_name] = "Billy Bob Junior";

    childrow = *(m_refTreeModel->append(row.children()));
    childrow[m_Columns.m_col_id] = 12;
    childrow[m_Columns.m_col_name] = "Sue Bob";
    */

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_id] = 2;
    row[m_Columns.m_col_name] = "Joey Jojo";
    row[m_Columns.m_col_extra] = "yadda";

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_id] = 3;
    row[m_Columns.m_col_name] = "Rob McRoberts";
    row[m_Columns.m_col_extra] = "";

    /*
    childrow = *(m_refTreeModel->append(row.children()));
    childrow[m_Columns.m_col_id] = 31;
    childrow[m_Columns.m_col_name] = "Xavier McRoberts";
    */

    // Add the model columns to the Combo (which is a kind of view),
    // rendering them in the default way:
    m_Combo.pack_start(m_Columns.m_col_id);
    m_Combo.pack_start(m_Columns.m_col_name);

    // An example of adding a cell renderer manually,
    // instead of using pack_start(model_column)
    // so we have more control:
    m_Combo.set_cell_data_func(
        m_cell, sigc::mem_fun(*this, &ExampleWindow::on_cell_data_extra));
    m_Combo.pack_start(m_cell);

    // Add the ComboBox to the window.
    add(m_Combo);

    // Connect signal handler:
    m_Combo.signal_changed().connect(
        sigc::mem_fun(*this, &ExampleWindow::on_combo_changed));

    show_all_children();
}

ExampleWindow::~ExampleWindow() {
    Gtk::TreeIter iList = m_refTreeModel->get_iter("0");
    while (iList) {
        std::cout << (*iList)[m_Columns.m_col_name] << std::endl;
        ++iList;
    }
    m_refTreeModel->clear();
}

void ExampleWindow::on_cell_data_extra(
    const Gtk::TreeModel::const_iterator& iter) {
    auto row = *iter;
    const Glib::ustring extra = row[m_Columns.m_col_extra];

    // Some arbitrary logic just to show that this is where you can do such
    // things:

    // Transform the value, deciding how to represent it as text:
    if (extra.empty())
        m_cell.property_text() = "(none)";
    else
        m_cell.property_text() = "-" + extra + "-";

    // Change other cell renderer properties too:
    m_cell.property_foreground() = (extra == "yadda" ? "red" : "green");
}

void ExampleWindow::on_combo_changed() {
    Gtk::TreeModel::iterator iter = m_Combo.get_active();
    if (iter) {
        Gtk::TreeModel::Row row = *iter;
        if (row) {
            // Get the data for the selected row, using our knowledge of the
            // tree model:
            int id = row[m_Columns.m_col_id];
            Glib::ustring name = row[m_Columns.m_col_name];

            std::cout << " ID=" << id << ", name=" << name << std::endl;
        }
    } else
        std::cout << "invalid iter" << std::endl;
}
