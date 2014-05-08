///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_python.rb
//

#include "interface.hh"

static PyObject* c_module;
static PyObject* py_module;
static PyObject* champ_module;

static void _init_python();

template <typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
PyObject* cxx2lang<PyObject*, int>(int in)
{
  return PyLong_FromLong(in);
}


template <>
PyObject* cxx2lang<PyObject*, std::string>(std::string in)
{
return PyUnicode_FromString (in.c_str());
}


template <>
PyObject* cxx2lang<PyObject*, bool>(bool in)
{
  return PyBool_FromLong(in);
}

template <typename Cxx>
PyObject* cxx2lang_array(const std::vector<Cxx>& in)
{
  size_t size = in.size();
  PyObject* out = PyList_New(size);

  for (unsigned int i = 0; i < size; ++i)
    PyList_SET_ITEM(out, i, (cxx2lang<PyObject*, Cxx>(in[i])));

  return out;
}

template <typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
int lang2cxx<PyObject*, int>(PyObject* in)
{
  long out = PyLong_AsLong(in);
  if (out == -1)
    if (PyErr_Occurred())
    {
      throw 42;
    }

  return out;
}

template <>
bool lang2cxx<PyObject*, bool>(PyObject* in)
{
  return (bool)lang2cxx<PyObject*, int>(in);
}

template <>
std::string lang2cxx<PyObject*, std::string>(PyObject* in)
{
  char * out = PyUnicode_AsUTF8(in);
  if (PyErr_Occurred())
    {
      throw 42;
    }
  return out;
}

template <typename Cxx>
std::vector<Cxx> lang2cxx_array(PyObject* in)
{
  if (!PyList_Check(in))
  {
    PyErr_SetString(PyExc_TypeError, "a list is required");
    throw 42;
  }

  std::vector<Cxx> out;
  unsigned int size = PyList_Size(in);

  for (unsigned int i = 0; i < size; ++i)
    out.push_back(lang2cxx<PyObject*, Cxx>(PyList_GET_ITEM(in, i)));

  return out;
}
///
// Information sur les cases
//
template <>
PyObject* cxx2lang<PyObject*, case_info>(case_info in)
{
  PyObject* name = PyUnicode_FromString("case_info");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
case_info lang2cxx<PyObject*, case_info>(PyObject* in)
{
  return (case_info)lang2cxx<PyObject*, int>(in);
}

///
// Erreurs possibles
//
template <>
PyObject* cxx2lang<PyObject*, erreur>(erreur in)
{
  PyObject* name = PyUnicode_FromString("erreur");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
erreur lang2cxx<PyObject*, erreur>(PyObject* in)
{
  return (erreur)lang2cxx<PyObject*, int>(in);
}

///
// Représente la position sur la carte
//
template <>
PyObject* cxx2lang<PyObject*, position>(position in)
{
  PyObject* tuple = PyTuple_New(2);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.x)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.y)));
  return tuple;
}

template <>
position lang2cxx<PyObject*, position>(PyObject* in)
{
  position out;
  PyObject* i;
  i = PyTuple_GetItem(in, 0);
  if (i == NULL) throw 42;
  out.x = lang2cxx<PyObject*, int>(i);
  i = PyTuple_GetItem(in, 1);
  if (i == NULL) throw 42;
  out.y = lang2cxx<PyObject*, int>(i);
  return out;
}

///
// Représente une tourelle
//
template <>
PyObject* cxx2lang<PyObject*, tourelle>(tourelle in)
{
  PyObject* tuple = PyTuple_New(5);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, position>(in.pos)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.portee)));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, int>(in.joueur)));
  PyTuple_SET_ITEM(tuple, 3, (cxx2lang<PyObject*, int>(in.vie)));
  PyTuple_SET_ITEM(tuple, 4, (cxx2lang<PyObject*, int>(in.attaque)));
  PyObject* name = PyUnicode_FromString("tourelle");
  PyObject* cstr = PyObject_GetAttr(py_module, name);
  Py_DECREF(name);
  if (cstr == NULL) throw 42;
  PyObject* ret = PyObject_CallObject(cstr, tuple);
  Py_DECREF(cstr);
  Py_DECREF(tuple);
  if (ret == NULL) throw 42;
  return ret;
}

template <>
tourelle lang2cxx<PyObject*, tourelle>(PyObject* in)
{
  tourelle out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pos = lang2cxx<PyObject*, position>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.portee = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.joueur = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(3);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.vie = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(4);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.attaque = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  return out;
}

///
// Retourne le type de la case à l'emplacement `pos`
//
static PyObject* p_info_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, case_info>(api_info_case(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne la liste des tourelles qui appartiennent au joueur ``joueur``
//
static PyObject* p_tourelles_joueur(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang_array(api_tourelles_joueur(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne la magie que possède le joueur ``joueur``
//
static PyObject* p_magie(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_magie(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne le nombre de sorciers du joueur ``joueur`` sur la case ``pos``
//
static PyObject* p_nb_sorciers(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_nb_sorciers(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

///
// Retourne le nombre de sorciers du joueur ``joueur`` déplacables sur la case ``pos``
//
static PyObject* p_nb_sorciers_deplacables(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_nb_sorciers_deplacables(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

///
// Retourne le numéro du joueur qui contrôle la case ``pos``
//
static PyObject* p_joueur_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_joueur_case(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne la tourelle située sur la case ``pos``
//
static PyObject* p_tourelle_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, tourelle>(api_tourelle_case(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne la position de la base du joueur ``joueur``
//
static PyObject* p_base_joueur(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, position>(api_base_joueur(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne vrai si l'on peut construire sur la case ``pos``
//
static PyObject* p_constructible(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, bool>(api_constructible(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

///
// Retourne la liste des positions constituant le plus court chemin allant de la case ``pos1`` à la case ``pos2``. Attention : Cette fonction est lente.
//
static PyObject* p_chemin(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang_array(api_chemin(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, position>(a1)));
  } catch (...) { return NULL; }
}

///
// Construire une tourelle à la position ``pos``
//
static PyObject* p_construire(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_construire(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

///
// Supprimer une tourelle à la position ``pos``
//
static PyObject* p_supprimer(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_supprimer(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Tirer avec ``pts`` points de dégats depuis la tourelles ``tourelle`` sur la position ``cible``
//
static PyObject* p_tirer(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
PyObject* a2;
  if (!PyArg_ParseTuple(args, "OOO", &a0, &a1, &a2)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_tirer(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, position>(a1), lang2cxx<PyObject*, position>(a2)));
  } catch (...) { return NULL; }
}

///
// Créer ``nb`` sorciers dans la base
//
static PyObject* p_creer(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_creer(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

///
// Déplace ``nb`` sorciers de la position ``depart`` jusqu'à la position ``arrivee``.
//
static PyObject* p_deplacer(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
PyObject* a2;
  if (!PyArg_ParseTuple(args, "OOO", &a0, &a1, &a2)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_deplacer(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, position>(a1), lang2cxx<PyObject*, int>(a2)));
  } catch (...) { return NULL; }
}

///
// Attaquer la tourelle à la position ``cible`` depuis la position ``pos``
//
static PyObject* p_assieger(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
PyObject* a2;
  if (!PyArg_ParseTuple(args, "OOO", &a0, &a1, &a2)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_assieger(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, position>(a1), lang2cxx<PyObject*, int>(a2)));
  } catch (...) { return NULL; }
}

///
// Retourne le numéro de votre joueur
//
static PyObject* p_moi(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_moi());
  } catch (...) { return NULL; }
}

///
// Retourne la liste des numéros de vos adversaires
//
static PyObject* p_adversaires(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_adversaires());
  } catch (...) { return NULL; }
}

///
// Retourne le numéro du tour actuel
//
static PyObject* p_tour_actuel(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_tour_actuel());
  } catch (...) { return NULL; }
}

///
// Retourne la distance entre deux positions
//
static PyObject* p_distance(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_distance(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, position>(a1)));
  } catch (...) { return NULL; }
}

///
// Annule la dernière action
//
static PyObject* p_annuler(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_annuler());
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type case_info
//
static PyObject* p_afficher_case_info(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_case_info(lang2cxx<PyObject*, case_info>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type erreur
//
static PyObject* p_afficher_erreur(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_erreur(lang2cxx<PyObject*, erreur>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type position
//
static PyObject* p_afficher_position(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_position(lang2cxx<PyObject*, position>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type tourelle
//
static PyObject* p_afficher_tourelle(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_tourelle(lang2cxx<PyObject*, tourelle>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

static PyObject* p_get_dump(PyObject* self, PyObject* args)
{
    char* dump = api_get_dump();
    PyObject* py_dump = PyUnicode_FromString(dump);
    return py_dump;
}


/*
** Api functions to register.
*/
static PyMethodDef api_callback[] = {
  {"get_dump", p_get_dump, METH_VARARGS, "get_dump"},  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC PyInit__api()
{
  static struct PyModuleDef apimoduledef = {
      PyModuleDef_HEAD_INIT,
      "_api",
      "API module",
      -1,
      api_callback,
      NULL,
      NULL,
      NULL,
      NULL,
  };
  return PyModule_Create(&apimoduledef);
}


/*
** Load a Python module
*/

static PyObject* _import_module(const char* m)
{
  PyObject* name = PyUnicode_FromString(m);
  PyObject* module = PyImport_Import(name);
  Py_DECREF(name);
  if (module == NULL)
    if (PyErr_Occurred())
    {
      PyErr_Print();
      abort();
    }
  return module;
}

/*
** Inititialize python, register API functions,
** and load .py file
*/
static void _init_python()
{
  static wchar_t empty_string[] = L"";
  static wchar_t *argv[] = { (wchar_t *) &empty_string, NULL };

  const char* champion_path;

  champion_path = getenv("CHAMPION_PATH");
  if (champion_path == NULL)
    champion_path = ".";

  setenv("PYTHONPATH", champion_path, 1);

  static wchar_t program_name[] = L"stechec";
  Py_SetProgramName(program_name);

  PyImport_AppendInittab("_api", PyInit__api);
  Py_Initialize();
  PySys_SetArgvEx(1, argv, 0);

  champ_module = _import_module("prologin");
  py_module = _import_module("api");
}

/*
** Run a python function.
*/
static PyObject* _call_python_function(const char* name)
{
  static bool initialized = false;

  if (!initialized)
  {
    initialized = true;
    _init_python();
  }

  PyObject *arglist, *func;
  PyObject *result = NULL;

  func = PyObject_GetAttrString(champ_module, (char*)name);
  if (func && PyCallable_Check(func))
  {
    arglist = Py_BuildValue("()");
    result = PyEval_CallObject(func, arglist);
    Py_XDECREF(arglist);
    Py_DECREF(func);
  }
  if (result == NULL && PyErr_Occurred())
    PyErr_Print();

  return result;
}

/*
** Functions called from stechec to C.
*/
extern "C" void partie_debut()
{
  PyObject* _retval = _call_python_function("partie_debut");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

extern "C" void phase_construction()
{
  PyObject* _retval = _call_python_function("phase_construction");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

extern "C" void phase_deplacement()
{
  PyObject* _retval = _call_python_function("phase_deplacement");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

extern "C" void phase_tirs()
{
  PyObject* _retval = _call_python_function("phase_tirs");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

extern "C" void phase_siege()
{
  PyObject* _retval = _call_python_function("phase_siege");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

extern "C" void partie_fin()
{
  PyObject* _retval = _call_python_function("partie_fin");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

