
/*! \mainpage Skypekit UiKit Class Reference
 *
 * \section intro_sec Introduction
 * 
 * The UiKit employs the Passive View form of the Model View
 * Presenter architecture. This architecture is reviewed in
 * detail by Martin Fowler in his review of GUI Architectures
 * at http://martinfowler.com/eaaDev/uiArchs.html#Model-view-presentermvp
 * 
 * In the MVP architecture, the View layer handles all details
 * of rendering, animation, and the mechanics of user
 * interactions. Qt is used extensively in this domain to
 * implement a portable, expressive set of widgets, controls,
 * and other user interface primitves.
 * 
 * The Presenter is responsible for all views. It establishes
 * views and their states, connects models to views, and
 * translates events from the interface into model and view
 * acitons. In UI Kit's implementation of the presenter layer,
 * we have used a state machine pattern that provides a rich
 * object model that represents different views well, and
 * allows a more objectified approach to maintaining and
 * synchronizing views and view state.
 * 
 * The Model layer encapsulates both the data: accounts,
 * contacts, conversations, etc. and the underlying events:
 * network activity such as incoming calls, chats, contact
 * changes, etc. The model interacts with SkypeKit
 * exclusively. For most critical models, the model maps
 * closely onto SkypeKit objects and functionality.
 * 
 * A salient feature of this architecture is testability.
 * Typically user interface code is difficult to test because
 * it is difficult to test the actual widgets and events in a
 * non-interactive test framework. MVP moves most of the
 * application business logic out of the view code and into the
 * presenter layer. Here the connection between events in the
 * view and decisions and actions implemented in the presenter
 * can be tested more easily by mocking view obects. For
 * instance, a user may initiate an account sign-off from a
 * menu, which in turn causes the presenter to close other open
 * views. One view might be an active call which will prompt
 * the user before terminatiing. These view interactions and
 * view state changes can now be usefully tested in the
 * presenter layer. The state machine pattern we have chosen
 * for our presenter is especially testable.
 * 
 * Another benefit of this architectural pattern is
 * flexibility. Many high level interface design decisions map
 * directly onto presenter objects. For instance, the same
 * models and the same interface widgets can be used to
 * implement multiple window interfaces or unified window
 * handling. The differences are largely implemented in
 * presenters that establish views and view states based on
 * these high level design choices. Another example to consider
 * is the handlng of active calls. We can implement UI or View
 * objects for full screen, floating window, in frame, sprite
 * (for HUD displays) or hidden. Those objects, and the
 * decisions about when to employ each view style and when to
 * transition between then, are implemented in the
 * presenter. Variations in view management can be implemented
 * as alternate presenter classes, and by encapsulating this
 * logic in the Presenter they become more easily testable.
 * 
 * MVP is not a panacea. That which is difficult to test - the
 * visual presentation of data and state on the screen, the
 * real time user interactions, etc. remains difficult to
 * test. But in adopting the Passive View variaton of MVP we
 * limit view objects to those interface level actions. The
 * mapping between those interface objects and the rest of the
 * application go through the Presenter, where we can now
 * establish meaningful, less brittle automated tests.
 * 
 * In the UiKit we have chosen to isolate the use of Qt to just
 * the View layer. Instead of using Qt's full framework, we
 * have chosen to use Qt just for interface widgets and
 * views. In the Model and Presenter classes, the Poco C++
 * framework has been adopted for such basic needs as events,
 * event handling, collections, managed pointers, etc. This
 * affords us some advantages. Poco is highly portable,
 * standard C++ . The resulting model and presenter modules
 * will be useful in embedded devices or in applicaitons that
 * use other UI frameworks and toolkits.
  *
 * \section install_sec Installation
 * 
 * See the README document in the source distribution for information
 * about building and running the qtsimpleclient client.
 */
