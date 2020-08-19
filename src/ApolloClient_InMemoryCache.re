module Types = ApolloClient_Types;

/**
 * Used on the client to rehydrate the cache using the initial data passed from the server
 * - e.g. window.__APOLLO_STATE__
 */
type restoreData;

/**
 * Define the data to pass to the restore method that'll be used used to rehydrate client.
 * If you don't want to pass any data, simply return `Js_null_undefined.undefined`.
 */
type inMemoryCacheRestoreData = Js.Nullable.t(restoreData);

/**
 * CreateInMemoryCache
 * https://github.com/apollographql/apollo-client/tree/master/packages/apollo-cache-inmemory
 */
/* Bind the InMemoryCache class */
[@bs.module "@apollo/client"] [@bs.new]
external apolloInMemoryCache: 'a => Types.apolloCache = "InMemoryCache";

/* Bind the restore method */
[@bs.send.pipe: 't]
external restore: inMemoryCacheRestoreData => Types.apolloCache = "restore";

/* Bind the extract method */
[@bs.send.pipe: 't]
external extractJs: bool => inMemoryCacheRestoreData = "extract";

let extract = (~optimistic=false, unit) => extractJs(optimistic);

type pagination;

[@bs.module "@apollo/client/utilities"]
external relayStylePagination: array(string) => pagination =
  "relayStylePagination";

/* Instantiate a new cache object */
[@bs.obj]
external makeApolloInMemoryCacheParams:
  (
    ~possibleTypes: Js.Dict.t(array(string))=?,
    ~dataIdFromObject: Js.t({..}) => string=?,
    ~typePolicies: Js.t({..})=?
  ) =>
  _;

let make = (~dataIdFromObject=?, ~possibleTypes=?, ~typePolicies=?, ()) => {
  /* Apollo Client, looks for key in Object. Doesn't check if value is null  */
  apolloInMemoryCache(
    makeApolloInMemoryCacheParams(
      ~dataIdFromObject?,
      ~possibleTypes?,
      ~typePolicies?,
    ),
  );
};
