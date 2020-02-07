### Make **clang-format** to skip section from fomatting

Use ` // clang-format off ` and ` // clang-format on ` to make it skip code sections.

```cpp
// clang-format off
// Don't touch this!
connect( m_Job, SIGNAL( error( const QString&, const QString& ) ),  this, SLOT( onError( const QString&, const QString& ) ) );
connect( m_Job, SIGNAL( message( const QString& ) ),                this, SLOT( onMessage( const QString& ) ) );
connect( m_Job, SIGNAL( progress( int, int ) ),                     this, SLOT( onProgress( int, int ) ) );
// clang-format on
// Carry on formatting
```