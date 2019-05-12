def main():
    #TODO: Make some usage docs, and add some ways to change arguments and options
    from clang.cindex import Index
    from clang.cindex import CursorKind
    from clang.cindex import Config
    from configure import conf

    # python bindings for libclang need to find libclang.so
    Config.set_library_file('/home/dismo/.vim/bundle/YouCompleteMe/third_party/ycmd/libclang.so.3.7')

    files = conf.search_headDir('./')

    def print_node_tree( node ):
        """ Uses libclangs cursor type to iterate over c/c++
            types and parse arguments
        """
        if node.kind == CursorKind.CXX_ACCESS_SPEC_DECL:
            print "%s : %s : " % ( node.access_specifier, node.access_specifier.name )
        if node.kind == CursorKind.TRANSLATION_UNIT:
            print ''
        print "%s : %s : %s" % ( node.kind.name, node.displayname, node.type.spelling )
        for child in node.get_children():
            if not child.location.file or child.location.file.name != testing:
                continue
            print_node_tree( child )

    index = Index.create()
    for f in files:
        testing = f
        tu = index.parse( f, args=['-x', 'c++'], options=0 ) # options=1 for include directories
        print_node_tree( tu.cursor )

if __name__ == '__main__':
    main()
